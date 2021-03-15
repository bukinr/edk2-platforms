/** @file
  Defines and prototypes for the IoTrap SMM driver

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _IO_TRAP_H_
#define _IO_TRAP_H_

//
// Include files
//
#include <Library/S3BootScriptLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include <Protocol/PchSmmIoTrapControl.h>
#include <Protocol/IoTrapExDispatch.h>

#define IO_TRAP_HANDLER_NUM 4

//
// Driver private data
//
#define IO_TRAP_INSTANCE_SIGNATURE  SIGNATURE_32 ('I', 'O', 'T', 'P')

typedef struct {
  EFI_HANDLE                            IoTrapHandle;
  /**
    The callback linked list for all "merged" IoTrap callbacks.
  **/
  LIST_ENTRY                            CallbackDataBase;
  /**
    The IoTrap IO range used length tracking for "merged" IoTrap register.
  **/
  UINT32                                TrapUsedLength;
  /**
    Determine if IoTrap can be merged with other IoTrap callbacks.
    If MergeDisable is TRUE, then there is only one callback function for one IoTrap register.
    If MergeDisable is FALSE, then there are multiple callbacks in the "CallbackDataBase" for one IoTrap register.
  **/
  BOOLEAN                               MergeDisable;
  /**
    Indicator of the resource tracking in ACPI.
    If the registration address is not 0, it's caller's responsibility to reserve the IO resource in ACPI.
  **/
  BOOLEAN                               ReservedAcpiIoResource;
  /**
    Dispatcher for each IoTrap register.
  **/
  PCH_SMI_DISPATCH_CALLBACK             CallbackDispatcher;
} IO_TRAP_ENTRY_ATTRIBUTES;

typedef struct {
  UINT32                                Signature;
  EFI_HANDLE                            Handle;
  EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL    EfiSmmIoTrapDispatchProtocol;
  PCH_SMM_IO_TRAP_CONTROL_PROTOCOL      PchSmmIoTrapControlProtocol;        ///< Protocol for runtime control the IoTrap state
  IO_TRAP_EX_DISPATCH_PROTOCOL          IoTrapExDispatchProtocol;           ///< Protocol for IoTrap Extension
  IO_TRAP_ENTRY_ATTRIBUTES              Entry[IO_TRAP_HANDLER_NUM];
} IO_TRAP_INSTANCE;

#define IO_TRAP_INSTANCE_FROM_THIS(a) CR (a, IO_TRAP_INSTANCE, EfiSmmIoTrapDispatchProtocol, IO_TRAP_INSTANCE_SIGNATURE)

///
/// "IOTRAP" RECORD
/// Linked list data structures
///
#define IO_TRAP_RECORD_SIGNATURE  SIGNATURE_32 ('I', 'T', 'R', 'C')

typedef struct _IO_TRAP_RECORD {
  UINT32                                    Signature;
  LIST_ENTRY                                Link;
  IO_TRAP_EX_REGISTER_CONTEXT               Context;
  /**
    The callback function of IoTrap protocol.
    This also indicate it's the record for IoTrapProtocol.
    Only one of IoTrapCallback or IoTrapExCallback is valid at a time.
  **/
  EFI_SMM_HANDLER_ENTRY_POINT2              IoTrapCallback;
  /**
    The callback function of IoTrapEx protocol
    This also indicate it's the record for IoTrapExProtocol.
    Only one of IoTrapCallback or IoTrapExCallback is valid at a time.
  **/
  IO_TRAP_EX_DISPATCH_CALLBACK              IoTrapExCallback;
  UINT8                                     IoTrapNumber;
} IO_TRAP_RECORD;

#define IO_TRAP_RECORD_FROM_LINK(_record) CR (_record, IO_TRAP_RECORD, Link, IO_TRAP_RECORD_SIGNATURE)

//
// Prototypes
//
/**
  The IoTrap module abstracts PCH I/O trapping capabilities for other drivers.
  This driver manages the limited I/O trap resources.

  @param[in] ImageHandle                Image handle for this driver image

  @retval EFI_SUCCESS                   Driver initialization completed successfully
**/
EFI_STATUS
EFIAPI
InstallIoTrap (
  IN EFI_HANDLE                     ImageHandle
  );

/**
  Register a new IO Trap SMI dispatch function with a parent SMM driver.
  The caller will provide information about the IO trap characteristics via
  the context.  This includes base address, length, read vs. r/w, etc.
  This function will autoallocate IO base address from a common pool if the base address is 0,
  and the RegisterContext Address field will be updated.
  The service will not perform GCD allocation if the base address is non-zero.
  In this case, the caller is responsible for the existence and allocation of the
  specific IO range.
  This function looks for the suitable handler and Register a new IoTrap handler
  if the IO Trap handler is not used. It also enable the IO Trap Range to generate
  SMI.

  @param[in] This                 Pointer to the  EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for
                                  this SMI source.
  @param[in, out] RegisterContext Pointer to the dispatch function's context.
                                  The caller fills this context in before calling
                                  the register function to indicate to the register
                                  function the IO trap SMI source for which the dispatch
                                  function should be invoked.  This may not be NULL.
                                  If the registration address is not 0, it's caller's responsibility
                                  to reserve the IO resource in ACPI.
  @param[out] DispatchHandle      Handle of dispatch function, for when interfacing
                                  with the parent SMM driver, will be the address of linked
                                  list link in the call back record.  This may not be NULL.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR        The driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources are available
  @retval EFI_INVALID_PARAMETER   Address requested is already in use.
  @retval EFI_ACCESS_DENIED       Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
IoTrapRegister (
  IN  CONST   EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL     *This,
  IN          EFI_SMM_HANDLER_ENTRY_POINT2           DispatchFunction,
  IN OUT      EFI_SMM_IO_TRAP_REGISTER_CONTEXT       *RegisterContext,
  OUT EFI_HANDLE                                      *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the  EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
  @retval EFI_ACCESS_DENIED       Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
IoTrapUnRegister (
  IN CONST  EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL    *This,
  IN EFI_HANDLE                                   DispatchHandle
  );

/**
  This I/O Trap SMI handler invokes the ACPI reference code to handle the SMI.
  It currently assumes it owns all of the IO trap SMI.

  @param[in] DispatchHandle           Not used

**/
VOID
EFIAPI
IoTrapCallback (
  IN  EFI_HANDLE                      DispatchHandle
  );

/**
  Pause IoTrap callback function.

  This function disables the SMI enable of IoTrap according to the DispatchHandle,
  which is returned by IoTrap callback registration. It only supports the DispatchHandle
  with MergeDisable TRUE and address not zero.

  @param[in] This                 Pointer to the PCH_SMM_IO_TRAP_CONTROL_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of the child service to change state.

  @retval EFI_SUCCESS             This operation is complete.
  @retval EFI_INVALID_PARAMETER   The DispatchHandle is invalid.
  @retval EFI_ACCESS_DENIED       The SMI status is alrady PAUSED.
**/
EFI_STATUS
EFIAPI
IoTrapControlPause (
  IN PCH_SMM_IO_TRAP_CONTROL_PROTOCOL * This,
  IN EFI_HANDLE                       DispatchHandle
  );

/**
  Resume IoTrap callback function.

  This function enables the SMI enable of IoTrap according to the DispatchHandle,
  which is returned by IoTrap callback registration. It only supports the DispatchHandle
  with MergeDisable TRUE and address not zero.

  @param[in] This                 Pointer to the PCH_SMM_IO_TRAP_CONTROL_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of the child service to change state.

  @retval EFI_SUCCESS             This operation is complete.
  @retval EFI_INVALID_PARAMETER   The DispatchHandle is invalid.
  @retval EFI_ACCESS_DENIED       The SMI status is alrady RESUMED.
**/
EFI_STATUS
EFIAPI
IoTrapControlResume (
  IN PCH_SMM_IO_TRAP_CONTROL_PROTOCOL * This,
  IN EFI_HANDLE                       DispatchHandle
  );

#endif
