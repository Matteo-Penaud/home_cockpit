/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file is generated by TouchGFX Generator 4.20.0.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN STM32TouchController */

#include <STM32TouchController.hpp>
#include "stm32h747i_eval_ts.h"

static bool bsp_ts_initialized = false;

void STM32TouchController::init()
{
    /**
     * Initialize touch controller and driver
     *
     */
    static const uint32_t width = 800;
    static const uint32_t height = 480;

    TS_Init_t config;
    config.Width = width;
    config.Height = height;
    config.Orientation = TS_SWAP_XY | TS_SWAP_Y;
    config.Accuracy = 0;

    if (BSP_TS_Init(0, &config) != BSP_ERROR_NONE)
    {
        return;
    }

    bsp_ts_initialized = true;
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    /**
     * By default sampleTouch returns false,
     * return true if a touch has been detected, otherwise false.
     *
     * Coordinates are passed to the caller by reference by x and y.
     *
     * This function is called by the TouchGFX framework.
     * By default sampleTouch is called every tick, this can be adjusted by HAL::setTouchSampleRate(int8_t);
     *
     */

    static TS_State_t state;

    if (!bsp_ts_initialized)
    {
        return false;
    }

    BSP_TS_GetState(0, &state);

    if (!state.TouchDetected)
    {
        return false;
    }

    x = state.TouchX;
    y = state.TouchY;
    return true;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
