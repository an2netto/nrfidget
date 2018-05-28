/**
 * App specific definitions
 *
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>\n

// <h> SS413A - Hall sensor

// <o> PIN_IN - Pin number  <0-47>


#ifndef PIN_IN
#define PIN_IN 13
#endif

// </h>
//==========================================================

// <h> 74HC595 - 8-bit shift register

// <o> SHREG_SER_PIN - Pin number  <0-47>


#ifndef SHREG_SER_PIN
#define SHREG_SER_PIN 25
#endif

// <o> SHREG_SRCLK_PIN - Pin number  <0-47>


#ifndef SHREG_SRCLK_PIN
#define SHREG_SRCLK_PIN 26
#endif

// <o> SHREG_RCLK_PIN - Pin number  <0-47>


#ifndef SHREG_RCLK_PIN
#define SHREG_RCLK_PIN 27
#endif

// </h>
//==========================================================

// <h> PCD8544 - Nokia display

// <h> SPI_PINOUT - SPI configuration

//==========================================================
// <o> PCD8544_CLK_PIN - Pin number  <0-47>


#ifndef PCD8544_CLK_PIN
#define PCD8544_CLK_PIN 19
#endif

// <o> PCD8544_DIN_PIN - Pin number  <0-47>


#ifndef PCD8544_DIN_PIN
#define PCD8544_DIN_PIN 17
#endif

// <o> PCD8544_SS_PIN - Pin number  <0-47>


#ifndef PCD8544_RST_PIN
#define PCD8544_RST_PIN 11
#endif

// <o> PCD8544_IRQ_PRIORITY  - Interrupt priority


// <i> Priorities 0,2 (nRF51) and 0,1,4,5 (nRF52) are reserved for SoftDevice
// <0=> 0 (highest)
// <1=> 1
// <2=> 2
// <3=> 3
// <4=> 4
// <5=> 5
// <6=> 6
// <7=> 7

#ifndef PCD8544_IRQ_PRIORITY
#define PCD8544_IRQ_PRIORITY 7
#endif

// </h>
//==========================================================

// <o> PCD8544_DC_PIN - Pin number  <0-47>


#ifndef PCD8544_DC_PIN
#define PCD8544_DC_PIN 15
#endif

// <o> PCD8544_HEIGHT - screen height  <0-320>


#ifndef PCD8544_HEIGHT
#define PCD8544_HEIGHT 48
#endif

// <o> PCD8544_WIDTH - screen width  <0-240>


#ifndef PCD8544_WIDTH
#define PCD8544_WIDTH 84
#endif

// <o> PCD8544_FONT - font

// <0=> Default (classic Roman style)
// <1=> Ada (classic Baroque style)
// <2=> Custom

#ifndef PCD8544_FONT
#define PCD8544_FONT 0
#endif

// <o> PCD8544_FONT_WIDTH - classic font width - 5px + 1px padding

#ifndef PCD8544_FONT_WIDTH
#define PCD8544_FONT_WIDTH 6
#endif

// <o> PCD8544_FONT_HEIGHT - classic font height

#ifndef PCD8544_FONT_HEIGHT
#define PCD8544_FONT_HEIGHT 8
#endif

// </h>

// <e> NRFX_SPIM_ENABLED - nrfx_spim - SPIM peripheral driver
//==========================================================
#ifndef NRFX_SPIM_ENABLED
#define NRFX_SPIM_ENABLED 1
#endif

// <q> NRFX_SPIM0_ENABLED  - Enable SPIM0 instance


#ifndef NRFX_SPIM0_ENABLED
#define NRFX_SPIM0_ENABLED 1
#endif

// <q> NRFX_SPIM1_ENABLED  - Enable SPIM1 instance


#ifndef NRFX_SPIM1_ENABLED
#define NRFX_SPIM1_ENABLED 1
#endif

// <q> NRFX_SPIM2_ENABLED  - Enable SPIM2 instance


#ifndef NRFX_SPIM2_ENABLED
#define NRFX_SPIM2_ENABLED 0
#endif

// <q> NRFX_SPIM3_ENABLED  - Enable SPIM3 instance


#ifndef NRFX_SPIM3_ENABLED
#define NRFX_SPIM3_ENABLED 0
#endif

// <q> NRFX_SPIM_EXTENDED_ENABLED  - Enable extended SPIM features


#ifndef NRFX_SPIM_EXTENDED_ENABLED
#define NRFX_SPIM_EXTENDED_ENABLED 0
#endif

// <o> NRFX_SPIM_MISO_PULL_CFG  - MISO pin pull configuration.

// <0=> NRF_GPIO_PIN_NOPULL
// <1=> NRF_GPIO_PIN_PULLDOWN
// <3=> NRF_GPIO_PIN_PULLUP

#ifndef NRFX_SPIM_MISO_PULL_CFG
#define NRFX_SPIM_MISO_PULL_CFG 0
#endif

// <o> NRFX_SPIM_DEFAULT_CONFIG_IRQ_PRIORITY  - Interrupt priority

// <0=> 0 (highest)
// <1=> 1
// <2=> 2
// <3=> 3
// <4=> 4
// <5=> 5
// <6=> 6
// <7=> 7

#ifndef NRFX_SPIM_DEFAULT_CONFIG_IRQ_PRIORITY
#define NRFX_SPIM_DEFAULT_CONFIG_IRQ_PRIORITY 7
#endif

// <e> NRFX_SPIM_CONFIG_LOG_ENABLED - Enables logging in the module.
//==========================================================
#ifndef NRFX_SPIM_CONFIG_LOG_ENABLED
#define NRFX_SPIM_CONFIG_LOG_ENABLED 0
#endif
// <o> NRFX_SPIM_CONFIG_LOG_LEVEL  - Default Severity level

// <0=> Off
// <1=> Error
// <2=> Warning
// <3=> Info
// <4=> Debug

#ifndef NRFX_SPIM_CONFIG_LOG_LEVEL
#define NRFX_SPIM_CONFIG_LOG_LEVEL 3
#endif

// <o> NRFX_SPIM_CONFIG_INFO_COLOR  - ANSI escape code prefix.

// <0=> Default
// <1=> Black
// <2=> Red
// <3=> Green
// <4=> Yellow
// <5=> Blue
// <6=> Magenta
// <7=> Cyan
// <8=> White

#ifndef NRFX_SPIM_CONFIG_INFO_COLOR
#define NRFX_SPIM_CONFIG_INFO_COLOR 0
#endif

// <o> NRFX_SPIM_CONFIG_DEBUG_COLOR  - ANSI escape code prefix.

// <0=> Default
// <1=> Black
// <2=> Red
// <3=> Green
// <4=> Yellow
// <5=> Blue
// <6=> Magenta
// <7=> Cyan
// <8=> White

#ifndef NRFX_SPIM_CONFIG_DEBUG_COLOR
#define NRFX_SPIM_CONFIG_DEBUG_COLOR 0
#endif

// </e>

// </e>

//==========================================================

// <<< end of configuration section >>>
#endif //APP_CONFIG_H
