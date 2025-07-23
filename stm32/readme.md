# STM32H747I-EVAL TBS

The default IDE is set to STM32CubeIDE, to change IDE open the STM32H747I-EVAL.ioc with STM32CubeMX and select from the supported IDEs (STM32CubeIDE, EWARM, and MDK-ARM). Supports flashing of the STM32H747I-EVAL board directly from TouchGFX Designer using GCC and STM32CubeProgrammer. Flashing the board requires STM32CubeProgrammer which can be downloaded from the ST webpage.

This TBS is configured for 480 x 800 pixels 24bpp screen resolution.

Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ - Pin PB8, RENDER_TIME - Pin PC0, FRAME_RATE - Pin PC1, MCU_ACTIVE - Pin PC2

## Dual-Core Changes

This TouchGFX Board Setup (TBS) supports the dual-core functionality of the STM32H747I-EVAL.
For this reason, it is important to understand that there are two separate sub-projects for each compiler:
- A CM4 project
- A CM7 project

Since TouchGFX is running on the CM7, its code is located within the CM7 folder inside the project structure:

- 📁 CM4
    - 📁Core
- 📁 CM7
    - 📁 Core
    - 📁 TouchGFX

The CM4 and CM7 sub-project are split up as below:
- 📁 STM32CubeIDE
    - 📁 CM4
    - 📁 CM7
