/** @file
  Configuration Manager Dxe

  Copyright (c) 2021 - 2023, ARM Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#include <IndustryStandard/DebugPort2Table.h>
#include <IndustryStandard/IoRemappingTable.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/ConfigurationManagerProtocol.h>

#include "ConfigurationManagerSoc.h"
#include "Platform.h"

EDKII_SOC_PLATFORM_REPOSITORY_INFO  MorelloSocRepositoryInfo = {
  // ACPI Table List
  {
    // FADT Table
    {
      EFI_ACPI_6_4_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_6_4_FIXED_ACPI_DESCRIPTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdFadt),
      NULL, // Optional pointer to the ACPI table data
      0,    // OemTableId
      0,    // OemRevision
      EFI_ACPI_6_4_FIXED_ACPI_DESCRIPTION_TABLE_MINOR_REVISION
    },
    // GTDT Table
    {
      EFI_ACPI_6_4_GENERIC_TIMER_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_6_4_GENERIC_TIMER_DESCRIPTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdGtdt),
      NULL
    },
    // MADT Table
    {
      EFI_ACPI_6_4_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_6_4_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdMadt),
      NULL
    },
    // SPCR Table
    {
      EFI_ACPI_6_4_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_SIGNATURE,
      EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdSpcr),
      NULL
    },
    // DSDT Table
    {
      EFI_ACPI_6_4_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
      0, // Unused
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdDsdt),
      (EFI_ACPI_DESCRIPTION_HEADER *)dsdtsoc_aml_code
    },
    // DBG2 Table
    {
      EFI_ACPI_6_4_DEBUG_PORT_2_TABLE_SIGNATURE,
      EFI_ACPI_DBG2_DEBUG_DEVICE_INFORMATION_STRUCT_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdDbg2),
      NULL
    },
    // PPTT Table
    {
      EFI_ACPI_6_4_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_STRUCTURE_SIGNATURE,
      EFI_ACPI_6_4_PROCESSOR_PROPERTIES_TOPOLOGY_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdPptt),
      NULL
    },
    // IORT Table
    {
      EFI_ACPI_6_4_IO_REMAPPING_TABLE_SIGNATURE,
      EFI_ACPI_IO_REMAPPING_TABLE_REVISION_00,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdIort),
      NULL
    },
    // PCI MCFG Table
    {
      EFI_ACPI_6_4_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE,
      EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_SPACE_ACCESS_TABLE_REVISION,
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdMcfg),
      NULL,
    },
    // SSDT table describing the PCI root complex
    {
      EFI_ACPI_6_4_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
      0, // Unused
      CREATE_STD_ACPI_TABLE_GEN_ID (EStdAcpiTableIdSsdt),
      (EFI_ACPI_DESCRIPTION_HEADER *)ssdtpcisoc_aml_code
    },
  },

  // GIC ITS
  {
    // GIC ITS - PCIe TCU
    {
      // The GIC ITS ID.
      0,
      // The physical address for the Interrupt Translation Service
      0x30060000,
      // Proximity Domain
      0
    },
    // GIC ITS - PCIe RC
    {
      // The GIC ITS ID.
      1,
      // The physical address for the Interrupt Translation Service
      0x300A0000,
      // Proximity Domain
      0
    },
    // GIC ITS - CCIX TCU
    {
      // The GIC ITS ID.
      2,
      // The physical address for the Interrupt Translation Service
      0x30040000,
      // Proximity Domain
      0
    },
    // GIC ITS - CCIX RC
    {
      // The GIC ITS ID.
      3,
      // The physical address for the Interrupt Translation Service
      0x30080000,
      // Proximity Domain
      0
    },
  },

  // ITS group node
  {
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (ItsGroupInfo[0]),
      // The number of ITS identifiers in the ITS node.
      1,
      // Reference token for the ITS identifier array
      REFERENCE_TOKEN_SOC (ItsIdentifierArray[0])
    },
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (ItsGroupInfo[1]),
      // The number of ITS identifiers in the ITS node.
      1,
      // Reference token for the ITS identifier array
      REFERENCE_TOKEN_SOC (ItsIdentifierArray[1])
    },
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (ItsGroupInfo[2]),
      // The number of ITS identifiers in the ITS node.
      1,
      // Reference token for the ITS identifier array
      REFERENCE_TOKEN_SOC (ItsIdentifierArray[2])
    },
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (ItsGroupInfo[3]),
      // The number of ITS identifiers in the ITS node.
      1,
      // Reference token for the ITS identifier array
      REFERENCE_TOKEN_SOC (ItsIdentifierArray[3])
    },
  },

  // ITS identifier array
  {
    {
      // The ITS Identifier
      0
    },
    {
      // The ITS Identifier
      1
    },
    {
      // The ITS Identifier
      2
    },
    {
      // The ITS Identifier
      3
    },
  },

  // SMMUv3 Node
  {
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (SmmuV3Info[0]),
      // Number of ID mappings
      2,
      // Reference token for the ID mapping array
      REFERENCE_TOKEN_SOC (DeviceIdMapping[0][0]),
      // SMMU Base Address
      0x4F400000,
      // SMMU flags
      EFI_ACPI_IORT_SMMUv3_FLAG_COHAC_OVERRIDE,
      // VATOS address
      0,
      // Model
      EFI_ACPI_IORT_SMMUv3_MODEL_GENERIC,
      // GSIV of the Event interrupt if SPI based
      267,
      // PRI Interrupt if SPI based
      72,
      // GERR interrupt if GSIV based
      269,
      // Sync interrupt if GSIV based
      268,
      // Proximity domain flag, ignored in this case
      0,
      // Index into the array of ID mapping
      1
    },
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (SmmuV3Info[1]),
      // Number of ID mappings
      2,
      // Reference token for the ID mapping array
      REFERENCE_TOKEN_SOC (DeviceIdMapping[2][0]),
      // SMMU Base Address
      0x4F000000,
      // SMMU flags
      EFI_ACPI_IORT_SMMUv3_FLAG_COHAC_OVERRIDE,
      // VATOS address
      0,
      // Model
      EFI_ACPI_IORT_SMMUv3_MODEL_GENERIC,
      // GSIV of the Event interrupt if SPI based
      260,
      // PRI Interrupt if SPI based
      73,
      // GERR interrupt if GSIV based
      262,
      // Sync interrupt if GSIV based
      261,
      // Proximity domain flag, ignored in this case
      0,
      // Index into the array of ID mapping
      1
    },
  },

  // Root Complex node info
  {
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (RootComplexInfo[0]),
      // Number of ID mappings
      1,
      // Reference token for the ID mapping array
      REFERENCE_TOKEN_SOC (DeviceIdMapping[1][0]),

      // Memory access properties : Cache coherent attributes
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,
      // Memory access properties : Allocation hints
      0,
      // Memory access properties : Memory access flags
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM,
      // ATS attributes
      EFI_ACPI_IORT_ROOT_COMPLEX_ATS_UNSUPPORTED,
      // PCI segment number
      0,
      // Memory address size limit
      42
    },
    {
      // Reference token for this Iort node
      REFERENCE_TOKEN_SOC (RootComplexInfo[1]),
      // Number of ID mappings
      1,
      // Reference token for the ID mapping array
      REFERENCE_TOKEN_SOC (DeviceIdMapping[1][1]),

      // Memory access properties : Cache coherent attributes
      EFI_ACPI_IORT_MEM_ACCESS_PROP_CCA,
      // Memory access properties : Allocation hints
      0,
      // Memory access properties : Memory access flags
      EFI_ACPI_IORT_MEM_ACCESS_FLAGS_CPM,
      // ATS attributes
      EFI_ACPI_IORT_ROOT_COMPLEX_ATS_UNSUPPORTED,
      // PCI segment number
      1,
      // Memory address size limit
      42
    },
  },

  // Array of Device ID mappings
  {
    // DeviceIdMapping[0][0] - [0][1]
    {
      /* Mapping SMMUv3 -> ITS Group
      */

      // SMMUv3 device ID mapping
      {
        // Input base
        0x0,
        // Number of input IDs
        0x0000FFFF,
        // Output Base
        0x0,
        // Output reference
        REFERENCE_TOKEN_SOC (ItsGroupInfo[1]),
        // Flags
        0
      },
      // SMMUv3 device ID mapping
      {
        // Input base
        0x0,
        // Number of input IDs
        0x00000001,
        // Output Base
        0x0,
        // Output reference token for the IORT node
        REFERENCE_TOKEN_SOC (ItsGroupInfo[0]),
        // Flags
        EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE
      }
    },
    // Device ID mapping [1][0] - [1][1]
    {
      // Mapping for  RootComplex -> SMMUv3
      {
        // Input base
        0x0,
        // Number of input IDs
        0x0000FFFF,
        // Output Base
        0x0,
        // Output reference
        REFERENCE_TOKEN_SOC (SmmuV3Info[0]),
        // Flags
        0
      },
      {
        // Input base
        0x0,
        // Number of input IDs
        0x0000FFFF,
        // Output Base
        0x0,
        // Output reference
        REFERENCE_TOKEN_SOC (SmmuV3Info[1]),
        // Flags
        0
      },
    },
    // DeviceIdMapping[2][0] - [2][1]
    {
      /* Mapping SMMUv3 -> ITS Group
      */
      // SMMUv3 device ID mapping
      {
        // Input base
        0x0,
        // Number of input IDs
        0x0000FFFF,
        // Output Base
        0x0,
        // Output reference
        REFERENCE_TOKEN_SOC (ItsGroupInfo[3]),
        // Flags
        0
      },
      // SMMUv3 device ID mapping
      {
        // Input base
        0x0,
        // Number of input IDs
        0x00000001,
        // Output Base
        0x0,
        // Output reference token for the IORT node
        REFERENCE_TOKEN_SOC (ItsGroupInfo[2]),
        // Flags
        EFI_ACPI_IORT_ID_MAPPING_FLAGS_SINGLE
      }
    },
  },
  // PCI Configuration Space Info
  {
    // PCIe ECAM
    {
      FixedPcdGet64 (PcdPciExpressBaseAddress),  // Base Address
      0x0,                                       // Segment Group Number
      FixedPcdGet32 (PcdPciBusMin),              // Start Bus Number
      FixedPcdGet32 (PcdPciBusMax)               // End Bus Number
    },
    // CCIX ECAM
    {
      FixedPcdGet64 (PcdCcixExpressBaseAddress),  // Base Address
      0x1,                                        // Segment Group Number
      FixedPcdGet32 (PcdCcixBusMin),              // Start Bus Number
      FixedPcdGet32 (PcdCcixBusMax)               // End Bus Number
    }
  },
};

EDKII_PLATFORM_REPOSITORY_INFO  MorelloRepositoryInfo = {
  &CommonPlatformInfo,
  &MorelloSocRepositoryInfo
};

/** Return a device Id mapping array.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       A token for identifying the object
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                              descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetDeviceIdMappingArray (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST This,
  IN  CONST CM_OBJECT_ID                                 CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                              Token,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST CmObject
  )
{
  EDKII_SOC_PLATFORM_REPOSITORY_INFO  *PlatformRepo;
  UINTN                               Count;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo->SocPlatRepoInfo;

  DEBUG ((DEBUG_INFO, "DeviceIdMapping - Token = %p\n"));

  if (Token == (CM_OBJECT_TOKEN)&PlatformRepo->DeviceIdMapping[0][0]) {
    Count = 2;
    DEBUG ((DEBUG_INFO, "DeviceIdMapping - Found DeviceIdMapping[0][0]\n"));
  } else if (Token ==
             (CM_OBJECT_TOKEN)&PlatformRepo->DeviceIdMapping[1][0])
  {
    Count = 1;
    DEBUG ((DEBUG_INFO, "DeviceIdMapping - Found DeviceIdMapping[1][0]\n"));
  } else if (Token ==
             (CM_OBJECT_TOKEN)&PlatformRepo->DeviceIdMapping[1][1])
  {
    Count = 1;
    DEBUG ((DEBUG_INFO, "DeviceIdMapping - Found DeviceIdMapping[1][1]\n"));
  } else if (Token ==
             (CM_OBJECT_TOKEN)&PlatformRepo->DeviceIdMapping[2][0])
  {
    Count = 2;
    DEBUG ((DEBUG_INFO, "DeviceIdMapping - Found DeviceIdMapping[2][0]\n"));
  } else {
    DEBUG ((DEBUG_INFO, "DeviceIdMapping - Not Found\n"));
    return EFI_NOT_FOUND;
  }

  CmObject->Data     = (VOID *)Token;
  CmObject->ObjectId = CmObjectId;
  CmObject->Count    = Count;
  CmObject->Size     = Count * sizeof (CM_ARM_ID_MAPPING);

  return EFI_SUCCESS;
}

/** Return an ITS identifier array.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       A token for identifying the object
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                              descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetItsIdentifierArray (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST This,
  IN  CONST CM_OBJECT_ID                                 CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                              Token,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST CmObject
  )
{
  EDKII_SOC_PLATFORM_REPOSITORY_INFO  *PlatformRepo;
  UINTN                               Count;
  UINTN                               Index;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo->SocPlatRepoInfo;

  Count = ARRAY_SIZE (PlatformRepo->ItsIdentifierArray);

  for (Index = 0; Index < Count; Index++) {
    if (Token == (CM_OBJECT_TOKEN)&PlatformRepo->ItsIdentifierArray[Index]) {
      CmObject->ObjectId = CmObjectId;
      CmObject->Size     = sizeof (PlatformRepo->ItsIdentifierArray[0]);
      CmObject->Data     = (VOID *)&PlatformRepo->ItsIdentifierArray[Index];
      CmObject->Count    = 1;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/** Return an ITS group info.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       A token for identifying the object
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                              descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetItsGroupInfo (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST This,
  IN  CONST CM_OBJECT_ID                                 CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                              Token,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST CmObject
  )
{
  EDKII_SOC_PLATFORM_REPOSITORY_INFO  *PlatformRepo;
  UINTN                               Count;
  UINTN                               Index;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo->SocPlatRepoInfo;

  Count = ARRAY_SIZE (PlatformRepo->ItsGroupInfo);

  for (Index = 0; Index < Count; Index++) {
    if (Token == (CM_OBJECT_TOKEN)&PlatformRepo->ItsGroupInfo[Index]) {
      CmObject->ObjectId = CmObjectId;
      CmObject->Size     = sizeof (PlatformRepo->ItsGroupInfo[0]);
      CmObject->Data     = (VOID *)&PlatformRepo->ItsGroupInfo[Index];
      CmObject->Count    = 1;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/** Return platform specific ARM namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetArmNameSpaceObjectPlat (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST This,
  IN  CONST CM_OBJECT_ID                                 CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                              Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST CmObject
  )
{
  EFI_STATUS                          Status;
  EDKII_SOC_PLATFORM_REPOSITORY_INFO  *PlatformRepo;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PlatformRepo = This->PlatRepoInfo->SocPlatRepoInfo;
  Status = EFI_NOT_FOUND;

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    case EArmObjGicItsInfo:
      Status = HandleCmObject (
                 CmObjectId,
                 &PlatformRepo->GicItsInfo,
                 sizeof (PlatformRepo->GicItsInfo),
                 ARRAY_SIZE (PlatformRepo->GicItsInfo),
                 CmObject
                 );
      break;

    case EArmObjSmmuV3:
      Status = HandleCmObject (
                 CmObjectId,
                 PlatformRepo->SmmuV3Info,
                 sizeof (PlatformRepo->SmmuV3Info),
                 ARRAY_SIZE (PlatformRepo->SmmuV3Info),
                 CmObject
                 );
      break;

    case EArmObjItsGroup:
      Status = HandleCmObjectRefByToken (
                 This,
                 CmObjectId,
                 PlatformRepo->ItsGroupInfo,
                 sizeof (PlatformRepo->ItsGroupInfo),
                 ARRAY_SIZE (PlatformRepo->ItsGroupInfo),
                 Token,
                 GetItsGroupInfo,
                 CmObject
                 );
      break;

    case EArmObjGicItsIdentifierArray:
      Status = HandleCmObjectRefByToken (
                 This,
                 CmObjectId,
                 PlatformRepo->ItsIdentifierArray,
                 sizeof (PlatformRepo->ItsIdentifierArray),
                 ARRAY_SIZE (PlatformRepo->ItsIdentifierArray),
                 Token,
                 GetItsIdentifierArray,
                 CmObject
                 );
      break;

    case EArmObjRootComplex:
      Status = HandleCmObject (
                 CmObjectId,
                 PlatformRepo->RootComplexInfo,
                 sizeof (PlatformRepo->RootComplexInfo),
                 ARRAY_SIZE (PlatformRepo->RootComplexInfo),
                 CmObject
                 );
      break;

    case EArmObjIdMappingArray:
      Status = HandleCmObjectRefByToken (
                 This,
                 CmObjectId,
                 PlatformRepo->DeviceIdMapping,
                 sizeof (PlatformRepo->DeviceIdMapping),
                 ARRAY_SIZE (PlatformRepo->DeviceIdMapping),
                 Token,
                 GetDeviceIdMappingArray,
                 CmObject
                 );
      break;

    case EArmObjPciConfigSpaceInfo:
      Status = HandleCmObject (
                 CmObjectId,
                 PlatformRepo->PciConfigInfo,
                 sizeof (PlatformRepo->PciConfigInfo),
                 ARRAY_SIZE (PlatformRepo->PciConfigInfo),
                 CmObject
                 );
      break;

    default:
    {
      break;
    }
  }// switch

  return Status;
}

/** Return platform specific standard namespace object.

  @param [in]      This        Pointer to the Configuration Manager Protocol.
  @param [in]      CmObjectId  The Configuration Manager Object ID.
  @param [in]      Token       An optional token identifying the object. If
                               unused this must be CM_NULL_TOKEN.
  @param [in, out] CmObject    Pointer to the Configuration Manager Object
                               descriptor describing the requested Object.

  @retval EFI_SUCCESS           Success.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
GetStandardNameSpaceObjectPlat (
  IN  CONST EDKII_CONFIGURATION_MANAGER_PROTOCOL  *CONST This,
  IN  CONST CM_OBJECT_ID                                 CmObjectId,
  IN  CONST CM_OBJECT_TOKEN                              Token OPTIONAL,
  IN  OUT   CM_OBJ_DESCRIPTOR                     *CONST CmObject
  )
{
  EFI_STATUS                          Status;
  EDKII_SOC_PLATFORM_REPOSITORY_INFO  *PlatformRepo;

  if ((This == NULL) || (CmObject == NULL)) {
    ASSERT (This != NULL);
    ASSERT (CmObject != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_NOT_FOUND;
  PlatformRepo = This->PlatRepoInfo->SocPlatRepoInfo;

  switch (GET_CM_OBJECT_ID (CmObjectId)) {
    case EStdObjAcpiTableList:
      Status = HandleCmObject (
                 CmObjectId,
                 PlatformRepo->CmAcpiTableList,
                 sizeof (PlatformRepo->CmAcpiTableList),
                 ARRAY_SIZE (PlatformRepo->CmAcpiTableList),
                 CmObject
                 );
      break;

    default:
    {
      break;
    }
  }

  return Status;
}
