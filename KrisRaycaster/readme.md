# STM32H750B-DK TBS

The default IDE is set to STM32CubeIDE, to change IDE open the STM32H750B-DK.ioc with STM32CubeMX and select from the supported IDEs (EWARM, MDK-ARM, and STM32CubeIDE). Supports flashing of the STM32H750B-DK board directly from TouchGFX Designer using GCC and STM32CubeProgrammer.Flashing the board requires STM32CubeProgrammer which can be downloaded from the ST webpage.

This TBS is configured for 272 x 480 pixels 16bpp screen resolution.

Performance testing can be done using the GPIO pins designated with the following signals: VSYNC_FREQ - Pin PG3 (D2), RENDER_TIME - Pin PB7 (D0), FRAME_RATE - Pin PB6 (D1), MCU_ACTIVE - Pin PA6 (D3).
