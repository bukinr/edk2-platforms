## @file
#  dsc include file for Platform Dxe
#
#  Copyright (c) 2020, 2023 ARM Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]

[BuildOptions.AARCH64__]
# Required for pre-processing Platform Dxe which includes platform-type specific build flags.
!ifdef $(ENABLE_MORELLO_CAP)
  *_CLANGDWARF_AARCH64_CC_FLAGS     = -DENABLE_MORELLO_CAP
  *_CLANGDWARF_AARCH64_PP_FLAGS     = -DENABLE_MORELLO_CAP
  *_CLANGDWARF_AARCH64_ASLCC_FLAGS  = -DENABLE_MORELLO_CAP
  *_CLANGDWARF_AARCH64_ASM_FLAGS    = -DENABLE_MORELLO_CAP -march=morello
!endif

[BuildOptions.AARCH64]
# Required for pre-processing Platform Dxe which includes platform-type specific build flags.
!ifdef $(ENABLE_MORELLO_CAP)
  *_CLANGDWARF_AARCH64_CC_FLAGS     = -DENABLE_MORELLO_CAP -march=morello -mabi=purecap
  *_CLANGDWARF_AARCH64_PP_FLAGS     = -DENABLE_MORELLO_CAP -march=morello -mabi=purecap -Wl,--no-nmagic,--local-caprelocs=legacy
  *_CLANGDWARF_AARCH64_ASLCC_FLAGS  = -DENABLE_MORELLO_CAP -march=morello -mabi=purecap -Wl,--no-nmagic,--local-caprelocs=legacy
  *_CLANGDWARF_AARCH64_ASM_FLAGS    = -DENABLE_MORELLO_CAP -march=morello -mabi=purecap -Wl,--no-nmagic,--local-caprelocs=legacy
  *_CLANGDWARF_AARCH64_DLINK_FLAGS  = -DENABLE_MORELLO_CAP -march=morello -mabi=purecap -Wl,--no-nmagic,--local-caprelocs=legacy
  *_CLANGDWARF_AARCH64_ASLDLINK_FLAGS  = -DENABLE_MORELLO_CAP -march=morello -mabi=purecap -Wl,--no-nmagic,--local-caprelocs=legacy
!endif