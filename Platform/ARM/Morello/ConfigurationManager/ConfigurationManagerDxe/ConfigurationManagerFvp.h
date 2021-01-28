/** @file

  Copyright (c) 2020 - 2021, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Glossary:
    - Cm or CM   - Configuration Manager
    - Obj or OBJ - Object
**/

#ifndef FVP_CONFIGURATION_MANAGER_H__
#define FVP_CONFIGURATION_MANAGER_H__

#include "ConfigurationManager.h"

/** C array containing the compiled AML template.
    These symbols are defined in the auto generated C file
    containing the AML bytecode array.
*/
extern CHAR8  dsdtfvp_aml_code[];

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
