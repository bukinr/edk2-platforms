/** @file

  Copyright (c) 2020, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#ifndef FVP_CONFIGURATION_MANAGER_H__
#define FVP_CONFIGURATION_MANAGER_H__

#include "ConfigurationManager.h"

//Add FVP Platform specific information
typedef struct FvpPlatformRepositoryInfo {
  /// List of ACPI tables
  CM_STD_OBJ_ACPI_TABLE_INFO            CmAcpiTableList[PLAT_ACPI_TABLE_COUNT];

} EDKII_FVP_PLATFORM_REPOSITORY_INFO;

typedef struct PlatformRepositoryInfo {

  EDKII_COMMON_PLATFORM_REPOSITORY_INFO   * CommonPlatRepoInfo;

  EDKII_FVP_PLATFORM_REPOSITORY_INFO      * FvpPlatRepoInfo;

} EDKII_PLATFORM_REPOSITORY_INFO;

extern EDKII_COMMON_PLATFORM_REPOSITORY_INFO CommonPlatformInfo;
#endif // FVP_CONFIGURATION_MANAGER_H__
