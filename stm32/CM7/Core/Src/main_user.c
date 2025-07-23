#include "main_user.h"

#include "stm32h747i_eval_lcd.h"

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

extern DSI_HandleTypeDef hdsi;

uint8_t pColLeft1[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
uint8_t pColRight1[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
uint8_t pPage1[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
uint8_t pScanCol1[]    = {0x02, 0x15};             /* Scan @ 533 */

static OTM8009A_Object_t OTM8009AObj;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;


void LCD_MspInit(void)
{
  // enable the LTDC clock 
  __HAL_RCC_LTDC_CLK_ENABLE();

  // toggle reset of LTDC IP 
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  // enable the DMA2D clock
  __HAL_RCC_DMA2D_CLK_ENABLE();

  // toggle reset of DMA2D IP 
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();

  // enable DSI host and wrapper clocks 
  __HAL_RCC_DSI_CLK_ENABLE();

  // soft reset the DSI Host and wrapper 
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  // NVIC configuration for LTDC interrupt that is now enabled 
  HAL_NVIC_SetPriority(LTDC_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  // NVIC configuration for DMA2D interrupt that is now enabled 
  HAL_NVIC_SetPriority(DMA2D_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

  // NVIC configuration for DSI interrupt that is now enabled 
  HAL_NVIC_SetPriority(DSI_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}


int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Size <= 1U)
  {
    if(HAL_DSI_ShortWrite(&hdsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  else
  {
    if(HAL_DSI_LongWrite(&hdsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}


int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_DSI_Read(&hdsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  return ret;
}


void dsiInit0(void)
{
  /* Toggle Hardware Reset of the DSI LCD using its XRES signal (active low) */
  BSP_LCD_Reset(0);
  
  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  LCD_MspInit();

  /* LCD clock configuration */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/19 = 42 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 42 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;    
  PeriphClkInitStruct.PLL3.PLL3N = 160;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;  
  PeriphClkInitStruct.PLL3.PLL3R = 19;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);   
}


void ltdcInit2a(void)
{
  OTM8009A_IO_t IOCtx;
  DSI_LPCmdTypeDef LPCmd;
  GPIO_InitTypeDef GPIO_Init_Structure;
  
  // start DSI 
  HAL_DSI_Start(&(hdsi));
    
  // initialize the OTM8009A LCD display IC driver 
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;
  OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx);
  OTM8009A_Init(&OTM8009AObj, OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);
  
  LPCmd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi, &LPCmd);
  
  HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA);
  HAL_DSI_ForceRXLowPower(&hdsi, ENABLE);  
  
  // enable GPIOJ clock 
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  
  // configure DSI_TE pin from MB1166: tearing effect on separated GPIO from KoD LCD
  // that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)
  // This pin is used only when the LCD and DSI link is configured in command mode
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);
}


void ltdcInit2b(void)
{
  __HAL_DSI_WRAPPER_DISABLE(&hdsi);  // disable DSI Wrapper in order to access and configure the LTDC 
  HAL_LTDC_SetPitch(&hltdc, 800, 0); // update pitch: the draw is done on the whole physical X Size 
  __HAL_DSI_WRAPPER_ENABLE(&hdsi);   // enable DSI Wrapper so DSI IP will drive the LTDC 
  
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft1);
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage1);
  
  // enable screen
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pScanCol1);
  HAL_DSI_ShortWrite(&(hdsi), 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPON, 0x00);
}


/**
 * @brief 
   @param pointer to error handler callback
 */
void quadSpiInit2(void (*error_handler)())
{
   BSP_QSPI_Init_t init ;
   init.InterfaceMode = MT25TL01G_QPI_MODE;
   init.TransferRate  = MT25TL01G_DTR_TRANSFER ;
   init.DualFlashMode = MT25TL01G_DUALFLASH_ENABLE;

   if (BSP_QSPI_Init(0, &init) != BSP_ERROR_NONE)
   {
      error_handler();
   }
   
   if (BSP_QSPI_EnableMemoryMappedMode(0) != BSP_ERROR_NONE)
   {
      error_handler();
   }
}


/**
 * @brief 
 */
void fmcInit1(void)
{
   // disable FMC Bank 1 to avoid speculative/cache accesses
   FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;
}


/**
 * @brief 
   @param pointer to error handler callback
 */
void fmcInit2(void (*error_handler)())
{
   BSP_SDRAM_DeInit(0);
   
   if(BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
   {
      error_handler();
   }
}


void bootModeSequence1(void (*error_handler)())
{
  int32_t timeout = 0xFFFF;
  
  // wait until CPU2 boots and enters in stop mode or timeout
  while ((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  
  if (timeout < 0)
  {
     error_handler();
  }
}


void bootModeSequence2(void (*error_handler)())
{
   int32_t timeout = 0xFFFF;
  
   // when system initialization is finished, Cortex-M7 will release Cortex-M4 by means of HSEM notification 
   // HW semaphore Clock enable
   __HAL_RCC_HSEM_CLK_ENABLE();
   
   // take HSEM 
   HAL_HSEM_FastTake(HSEM_ID_0);
   
   // release HSEM in order to notify the CPU2 (CM4)
   HAL_HSEM_Release(HSEM_ID_0,0);
   
   // wait until CPU2 wakes up from stop mode 
   while ((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
   
   if (timeout < 0)
   {
      error_handler();
   }
}
