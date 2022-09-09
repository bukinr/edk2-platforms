/** @file

  Copyright (c) 2021 - 2023, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <MorelloPlatform.h>

// The total number of descriptors, including the final "end-of-table" descriptor.
#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS \
  ( \
    18 \
    + ((FixedPcdGet32 (PcdPlatformGopBufferSize) != 0) ? 1 : 0) \
  )

#if !defined(MDEPKG_NDEBUG)
STATIC CONST CHAR8 *gTblAttrDesc[] = {
  "UNCACHED_UNBUFFERED          ",
  "NONSECURE_UNCACHED_UNBUFFERED",
  "WRITE_BACK                   ",
  "NONSECURE_WRITE_BACK         ",
  "WB_NONSHAREABLE              ",
  "NONSECURE_WB_NONSHAREABLE    ",
  "WRITE_THROUGH                ",
  "NONSECURE_WRITE_THROUGH      ",
  "DEVICE                       ",
  "NONSECURE_DEVICE             "
};
#endif

#define LOG_MEM(desc) DEBUG ((                                              \
                        DEBUG_ERROR,                                        \
                        desc,                                               \
                        VirtualMemoryTable[Index].PhysicalBase,             \
                        (VirtualMemoryTable[Index].PhysicalBase +           \
                         VirtualMemoryTable[Index].Length - 1),             \
                        VirtualMemoryTable[Index].Length,                   \
                        gTblAttrDesc[VirtualMemoryTable[Index].Attributes]  \
                        ));

/**
  Returns the Virtual Memory Map of the platform.

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU
  on your platform.

  @param[out] VirtualMemoryMap Array of ARM_MEMORY_REGION_DESCRIPTOR describing
                               a Physical-to-Virtual Memory mapping. This array
                               must be ended by a zero-filled entry.
**/
VOID
ArmPlatformGetVirtualMemoryMap (
  OUT ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_DESCRIPTOR  *VirtualMemoryTable;
  CONST MORELLO_PLAT_INFO_FVP   *PlatInfo;
  EFI_RESOURCE_ATTRIBUTE_TYPE   ResourceAttributes;
  EFI_STATUS                    Status;
  UINT64                        DramBlock2Size;
  UINTN                         Index;

  Status = PeiServicesLocatePpi (
             &gArmMorelloFvpPlatformInfoDescriptorPpiGuid,
             0,
             NULL,
             (VOID **)&PlatInfo
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[%a]: failed to locate gArmMorelloFvpPlatformInfoDescriptorPpiGuid - %r\n",
      gEfiCallerBaseName,
      Status
      ));
    return;
  }

  Index          = 0;
  DramBlock2Size = 0;

  if (PlatInfo->LocalDdrSize > MORELLO_DRAM_BLOCK1_SIZE) {
    DramBlock2Size = PlatInfo->LocalDdrSize - MORELLO_DRAM_BLOCK1_SIZE;
  }

  if (DramBlock2Size != 0) {
    ResourceAttributes =
      EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
      EFI_RESOURCE_ATTRIBUTE_TESTED;

    BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
      ResourceAttributes,
      FixedPcdGet64 (PcdDramBlock2Base),
      DramBlock2Size
      );
  }

  ASSERT (VirtualMemoryMap != NULL);

  VirtualMemoryTable = AllocatePool (sizeof (ARM_MEMORY_REGION_DESCRIPTOR) *
                                     MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  if (VirtualMemoryTable == NULL) {
    return;
  }

  DEBUG ((
    DEBUG_ERROR,
    " Memory Map\n----------------------------------------------------------\n"
    ));
  DEBUG ((
    DEBUG_ERROR,
    "Description                     :        START       -        END         " \
    "[        SIZE        ] {              ATTR             }\n"
    ));

  // SubSystem Peripherals - Generic Watchdog
  VirtualMemoryTable[Index].PhysicalBase    = MORELLO_GENERIC_WDOG_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_GENERIC_WDOG_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_GENERIC_WDOG_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("Generic Watchdog                : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Peripherals - REFCLK CNTRead
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_REFCLK_CNT_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_REFCLK_CNT_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_REFCLK_CNT_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("REFCLK CNTRead                  : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Peripherals - AP_REFCLK CNTCTL
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_AP_REFCLK_CNT_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_AP_REFCLK_CNT_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_AP_REFCLK_CNT_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("AP_REFCLK CNTCL                 : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Peripherals - AP_REFCLK_NS CNTCTL
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_AP_REFCLK_NS_CNT_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_AP_REFCLK_NS_CNT_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_AP_REFCLK_NS_CNT_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("AP_REFCLK_NS CNTCTL             : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Peripherals - GIC-600
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_GIC_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_GIC_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_GIC_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("GIC-600                         : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Peripherals - GICITS-600
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_GICITS_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_GICITS_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_GICITS_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("GICITS-600                      : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Peripherals - GICR-600
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_GICR_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_GICR_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_GICR_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("GICR-600                        : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Peripherals - SMMU
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_SMMU_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_SMMU_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_SMMU_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("SMMU                            : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem non-secure SRAM
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_NON_SECURE_SRAM_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_NON_SECURE_SRAM_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_NON_SECURE_SRAM_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;
  LOG_MEM ("non-secure SRAM                 : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // SubSystem Pheripherals - UART0
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_UART0_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_UART0_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_UART0_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("UART0                           : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  VirtualMemoryTable[++Index].PhysicalBase  = FixedPcdGet32 (PcdArmMaliDxxBase);
  VirtualMemoryTable[Index].VirtualBase     = FixedPcdGet32 (PcdArmMaliDxxBase);
  VirtualMemoryTable[Index].Length          = FixedPcdGet32 (PcdArmMaliDxxSize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("Mali D71                        : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // DDR Primary
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdSystemMemoryBase);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdSystemMemorySize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
  LOG_MEM ("DDR Primary                     : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  if (FixedPcdGet32 (PcdPlatformGopBufferSize) != 0) {
    ASSERT (
      (PcdGet64 (PcdSystemMemoryBase) +
       PcdGet64 (PcdSystemMemorySize) - 1) <
      FixedPcdGet64 (PcdPlatformGopBufferBase)
      );
    VirtualMemoryTable[++Index].PhysicalBase = FixedPcdGet64 (PcdPlatformGopBufferBase);
    VirtualMemoryTable[Index].VirtualBase    = FixedPcdGet64 (PcdPlatformGopBufferBase);
    VirtualMemoryTable[Index].Length         = FixedPcdGet32 (PcdPlatformGopBufferSize);
    // Map as Normal Non-Cacheable memory, so that we can use the accelerated
    // SetMem/CopyMem routines that may use unaligned accesses or
    // DC ZVA instructions. If mapped as device memory, these routine may cause
    // alignment faults.
    // NOTE: The attribute value is misleading, it indicates memory map type as
    // an un-cached, un-buffered but allows buffering and reordering.
    VirtualMemoryTable[Index].Attributes = ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED;
    LOG_MEM ("DDR GOP carve out               : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");
  }

  // DDR Secondary
  if (DramBlock2Size != 0) {
    VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdDramBlock2Base);
    VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdDramBlock2Base);
    VirtualMemoryTable[Index].Length          = DramBlock2Size;
    VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;
    LOG_MEM ("DDR Secondary                   : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");
  }

  // Expansion Peripherals
  VirtualMemoryTable[++Index].PhysicalBase  = MORELLO_EXP_PERIPH_BASE;
  VirtualMemoryTable[Index].VirtualBase     = MORELLO_EXP_PERIPH_BASE;
  VirtualMemoryTable[Index].Length          = MORELLO_EXP_PERIPH_SZ;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("Expansion Peripherals           : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // PCI Configuration Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdPciExpressBaseAddress);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdPciExpressBaseAddress);
  VirtualMemoryTable[Index].Length          = (FixedPcdGet32 (PcdPciBusMax) -
                                               FixedPcdGet32 (PcdPciBusMin) + 1) *
                                               SIZE_1MB;
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("PCI Configuration Space         : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // PCI MMIO32/IO Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet32 (PcdPciMmio32Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet32 (PcdPciMmio32Base);
  VirtualMemoryTable[Index].Length          = PcdGet32 (PcdPciMmio32Size) +
                                              PcdGet32 (PcdPciIoSize);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("PCI MMIO32 & IO Region          : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // PCI MMIO64 Space
  VirtualMemoryTable[++Index].PhysicalBase  = PcdGet64 (PcdPciMmio64Base);
  VirtualMemoryTable[Index].VirtualBase     = PcdGet64 (PcdPciMmio64Base);
  VirtualMemoryTable[Index].Length          = PcdGet64 (PcdPciMmio64Size);
  VirtualMemoryTable[Index].Attributes      = ARM_MEMORY_REGION_ATTRIBUTE_DEVICE;
  LOG_MEM ("PCI MMIO64 Region               : 0x%016lx - 0x%016lx [ 0x%016lx ] { %a }\n");

  // End of Table
  VirtualMemoryTable[++Index].PhysicalBase  = 0;
  VirtualMemoryTable[Index].VirtualBase     = 0;
  VirtualMemoryTable[Index].Length          = 0;
  VirtualMemoryTable[Index].Attributes      = (ARM_MEMORY_REGION_ATTRIBUTES)0;

  ASSERT ((Index) < MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);
  DEBUG ((DEBUG_INIT, "Virtual Memory Table setup complete.\n"));

  *VirtualMemoryMap = VirtualMemoryTable;
}
