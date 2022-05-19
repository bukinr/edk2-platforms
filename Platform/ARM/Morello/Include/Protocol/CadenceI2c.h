/** @file
  Cadence I2C controller UEFI protocol(s)

  Copyright (c) 2022, ARM Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef GUARD__FBB25ABF865442729D7A6A3B32A31F68_
#define GUARD__FBB25ABF865442729D7A6A3B32A31F68_

#include <Uefi.h>

/// This protocol contains the relevant information to install a
/// EFI_I2C_MASTER_PROTOCOL to control one Cadence I2C controller
typedef struct {
  /// Base MMIO address
  EFI_PHYSICAL_ADDRESS    MmioBase;

  /// Input hardware clock in Hertz
  UINT32                  InputClockHz;
} CADENCE_I2C_INSTALL_PROTOCOL;

#endif // GUARD__FBB25ABF865442729D7A6A3B32A31F68_
