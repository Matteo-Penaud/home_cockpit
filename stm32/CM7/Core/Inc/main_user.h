#ifndef __MAIN_CUSTOM_H
#define __MAIN_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h747i_eval_qspi.h"
#include "stm32h747i_eval_sdram.h"
#include "stm32h747i_eval_bus.h"
#include "stm32h747i_eval_errno.h"
#include "../Components/otm8009a/otm8009a.h"

//! dsi write callback
extern int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);

//! dsi read callback
extern int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);

//! dsi host initialization
void dsiHostInit0(void);

//! quad spi initialization
void quadSpiInit2(void (*error_handler)());

//! fmc initialization step 1
void fmcInit1(void);

//! fmc initialization step 2
void fmcInit2(void (*error_handler)());

//! boot mode sequence step 1
void bootModeSequence1(void (*error_handler)());

//! boot mode sequence step 2
void bootModeSequence2(void (*error_handler)());

//! dsi initialization step 0
void dsiInit0(void);

//! ltdc initialization step 2 pt1
void ltdcInit2a(void);

//! ltdc initialization step 2 pt2
void ltdcInit2b(void);

#ifdef __cplusplus
}
#endif

#endif 
