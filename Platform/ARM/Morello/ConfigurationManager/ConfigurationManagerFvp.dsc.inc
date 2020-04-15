## @file
#  dsc include file for Configuration Manager
#
#  Copyright (c) 2020, ARM Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]

[BuildOptions]
# Required for pre-processing ASL files that include platform specific headers.
  *_*_*_ASLPP_FLAGS        = $(PLATFORM_FLAGS)

[Components.common]
  # Configuration Manager
  Platform/ARM/Morello/ConfigurationManager/ConfigurationManagerDxe/ConfigurationManagerDxeFvp.inf {
    <LibraryClasses>
    # Platform ASL Tables
    PlatformAslTablesLib|Platform/ARM/Morello/ConfigurationManager/PlatformASLTablesLib/PlatformASLTablesLibFvp.inf
  <BuildOptions>
   *_*_*_PLATFORM_FLAGS = -I$(BIN_DIR)/Platform/ARM/Morello/ConfigurationManager/PlatformASLTablesLib/PlatformASLTablesLibFvp/OUTPUT
  }
