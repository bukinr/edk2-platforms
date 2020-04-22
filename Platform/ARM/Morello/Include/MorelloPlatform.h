/** @file
*
*  Copyright (c) 2020, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef MORELLO_PLATFORM_H__
#define MORELLO_PLATFORM_H__

#define MORELLO_DRAM_BLOCK1_SIZE               SIZE_2GB

// ****************************************************************************
// Platform Memory Map
// ****************************************************************************

// SubSystem Peripherals - UART0
#define MORELLO_UART0_BASE                     0x2A400000
#define MORELLO_UART0_SZ                       SIZE_64KB

// SubSystem Peripherals - UART1
#define MORELLO_UART1_BASE                     0x2A410000
#define MORELLO_UART1_SZ                       SIZE_64KB

// SubSystem Peripherals - Generic Watchdog
#define MORELLO_GENERIC_WDOG_BASE              0x2A440000
#define MORELLO_GENERIC_WDOG_SZ                SIZE_128KB

// SubSystem Peripherals - GIC(600)
#define MORELLO_GIC_BASE                       0x30000000
#define MORELLO_GICR_BASE                      0x300C0000
#define MORELLO_GIC_SZ                         SIZE_256KB
#define MORELLO_GICR_SZ                        SIZE_1MB

// SubSystem non-secure SRAM
#define MORELLO_NON_SECURE_SRAM_BASE           0x06000000
#define MORELLO_NON_SECURE_SRAM_SZ             SIZE_64KB

// AXI Expansion peripherals
#define MORELLO_EXP_PERIPH_BASE                0x1C000000
#define MORELLO_EXP_PERIPH_BASE_SZ             0x1300000

// Platform information structure base address
#define MORELLO_PLAT_INFO_STRUCT_BASE          (MORELLO_NON_SECURE_SRAM_BASE + \
                                                0x00008000)

/*
 * Platform information structure stored in Non-secure SRAM. Platform
 * information are passed from the trusted firmware with the below structure
 * format. The elements of MORELLO_PLAT_INFO should be always in sync with
 * the lower level firmware.
 */
typedef struct {
  /*! 0 - Single Chip, 1 - Chip to Chip (C2C) */
  UINT8   Mode;
  /*! Slave count in C2C mode */
  UINT8   SlaveCount;
  /*! Local DDR memory size in GigaBytes */
  UINT8   LocalDdrSize;
  /*! Remote DDR memory size in GigaBytes */
  UINT8   RemoteDdrSize;
} MORELLO_PLAT_INFO;

#endif
