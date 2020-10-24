/** @file
*  Differentiated System Description Table Fields (DSDT)
*
*  Copyright (c) 2020, ARM Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

DefinitionBlock("Dsdt.aml", "DSDT", 1, "ARMLTD", "MORELLO", 0x20181101) {
  Scope(_SB) {
    Device(CP00) { // Cluster 0, Cpu 0
      Name(_HID, "ACPI0007")
      Name(_UID, 0)
      Name(_STA, 0xF)
    }

    Device(CP01) { // Cluster 0, Cpu 1
      Name(_HID, "ACPI0007")
      Name(_UID, 1)
      Name(_STA, 0xF)
    }

    Device(CP02) { // Cluster 1, Cpu 0
      Name(_HID, "ACPI0007")
      Name(_UID, 2)
      Name(_STA, 0xF)
    }

    Device(CP03) { // Cluster 1, Cpu 1
      Name(_HID, "ACPI0007")
      Name(_UID, 3)
      Name(_STA, 0xF)
    }

    // UART PL011
    Device(COM1) {
      Name(_HID, "ARMH0011")
      Name(_CID, "PL011")
      Name(_UID, 0)

      Method(_STA) {
        Return(0xF)
      }

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x2A400000, 0x1000)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 95 }
      })
    }

    // VIRTIO DISK
    Device(VR00) {
      Name(_HID, "LNRO0005")
      Name(_UID, 0)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(
          ReadWrite,
          FixedPcdGet32(PcdVirtioBlkBaseAddress),
          FixedPcdGet32(PcdVirtioBlkSize)
        )
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) {
          FixedPcdGet32(PcdVirtioBlkInterrupt)
        }
      })
    }

    // VIRTIO RANDOM
    Device(VR01) {
      Name(_HID, "LNRO0005")
      Name(_UID, 0)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x1C190000, 0x00000200)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 133 }
      })
    }

    // SMC91X
    Device(NET0) {
      Name(_HID, "LNRO0003")
      Name(_UID, 0)

      Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, 0x1D100000, 0x00001000)
        Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 130 }
      })
    }
  } // Scope(_SB)
}
