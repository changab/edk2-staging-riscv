/** @file
  RISC-V DXE IPL to DXE core handoff platform library using OpenSBI

  Copyright (c) 2019 - 2020, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Core/DxeIplPeim/DxeIpl.h>
#include <IndustryStandard/RiscVOpensbi.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_init.h>
#include <sbi/sbi_scratch.h>
#include <Library/RiscVCpuLib.h>
#include <Library/RiscVPlatformDxeIpl.h>

/**
   RISC-V platform DXE IPL to DXE OpenSBI mdoe switch handler.
   This function is executed in RISC-V Supervisor mode.

   This function performs a CPU architecture specific operations to execute
   the entry point of DxeCore with the parameters of HobList.
   It also installs EFI_END_OF_PEI_PPI to signal the end of PEI phase.

   @param BaseOfStack        Base address of stack
   @param TopOfStack         Top address of stack
   @param DxeCoreEntryPoint  The entry point of DxeCore.
   @param HobList            The start of HobList passed to DxeCore.

**/
VOID
RiscVDxeIplHandoffOpenSbiHandler (
  IN UINTN HardId,
  IN OPENSBI_SWITCH_MODE_CONTEXT *ThisSwitchContext
  )
{
  DEBUG ((DEBUG_INFO, "OpenSBI mode switch DXE IPL Handoff handler entry\n"));

  SwitchStack (
    (SWITCH_STACK_ENTRY_POINT)(UINTN)ThisSwitchContext->DxeCoreEntryPoint,
    ThisSwitchContext->HobList.Raw,
    NULL,
    ThisSwitchContext->TopOfStack
    );

  //
  // Shold never came back.
  //
  UNREACHABLE();
}


/**
   RISC-V platform DXE IPL to DXE core handoff process.

   This function performs a CPU architecture specific operations to execute
   the entry point of DxeCore with the parameters of HobList.
   It also installs EFI_END_OF_PEI_PPI to signal the end of PEI phase.

   @param BaseOfStack        Base address of stack
   @param TopOfStack         Top address of stack
   @param DxeCoreEntryPoint  The entry point of DxeCore.
   @param HobList            The start of HobList passed to DxeCore.

**/
VOID
RiscVPlatformHandOffToDxeCore (
  IN VOID *BaseOfStack,
  IN VOID *TopOfStack,
  IN EFI_PHYSICAL_ADDRESS DxeCoreEntryPoint,
  IN EFI_PEI_HOB_POINTERS HobList
  )
{
  EFI_STATUS Status;
  struct sbi_scratch *ThisScratch;
  OPENSBI_SWITCH_MODE_CONTEXT OpenSbiSwitchModeContext;

  //
  // End of PEI phase signal
  //
  Status = PeiServicesInstallPpi (&gEndOfPeiSignalPpi);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "DXE IPL to DXE Core using OpenSBI\n"));
  //
  // Setup next address in OpenSBI scratch
  //
  OpenSbiSwitchModeContext.BaseOfStack = BaseOfStack;
  OpenSbiSwitchModeContext.TopOfStack = TopOfStack;
  OpenSbiSwitchModeContext.HobList = HobList;
  OpenSbiSwitchModeContext.DxeCoreEntryPoint = DxeCoreEntryPoint;
  ThisScratch = sbi_scratch_thishart_ptr ();
  ThisScratch->next_arg1 = (UINTN)&OpenSbiSwitchModeContext;
  ThisScratch->next_addr = (UINTN)RiscVDxeIplHandoffOpenSbiHandler;
  ThisScratch->next_mode = PRV_S;

  DEBUG ((DEBUG_INFO, "          Base address of satck: 0x%x\n", BaseOfStack));
  DEBUG ((DEBUG_INFO, "          Top address of satck: 0x%x\n", TopOfStack));
  DEBUG ((DEBUG_INFO, "          HOB list address: 0x%x\n", &HobList));
  DEBUG ((DEBUG_INFO, "          DXE core entry pointer: 0x%x\n", DxeCoreEntryPoint));
  DEBUG ((DEBUG_INFO, "          OpenSBI Switch mode arg1: 0x%x\n", (UINTN)&OpenSbiSwitchModeContext));
  DEBUG ((DEBUG_INFO, "          OpenSBI Switch mode handler address: 0x%x\n", (UINTN)RiscVDxeIplHandoffOpenSbiHandler));
  DEBUG ((DEBUG_INFO, "          OpenSBI Switch mode to privilege 0x%x\n", PRV_S));
  sbi_init (ThisScratch);
}

/**
   Transfers control to DxeCore.

   This function performs a CPU architecture specific operations to execute
   the entry point of DxeCore with the parameters of HobList.
   It also installs EFI_END_OF_PEI_PPI to signal the end of PEI phase.

   @param DxeCoreEntryPoint         The entry point of DxeCore.
   @param HobList                   The start of HobList passed to DxeCore.

**/
VOID
HandOffToDxeCore (
  IN EFI_PHYSICAL_ADDRESS   DxeCoreEntryPoint,
  IN EFI_PEI_HOB_POINTERS   HobList
  )
{
  VOID                            *BaseOfStack;
  VOID                            *TopOfStack;
  EFI_STATUS                      Status;
  //
  //
  // Allocate 128KB for the Stack
  //
  BaseOfStack = AllocatePages (EFI_SIZE_TO_PAGES (STACK_SIZE));
  if (BaseOfStack == NULL) {
    DEBUG((DEBUG_ERROR, "%a: Can't allocate memory for stack.", __FUNCTION__));
    ASSERT(FALSE);
  }

  //
  // Compute the top of the stack we were allocated. Pre-allocate a UINTN
  // for safety.
  //
  TopOfStack = (VOID *)((UINTN) BaseOfStack + EFI_SIZE_TO_PAGES (STACK_SIZE) * EFI_PAGE_SIZE - CPU_STACK_ALIGNMENT);
  TopOfStack = ALIGN_POINTER (TopOfStack, CPU_STACK_ALIGNMENT);

  //
  // End of PEI phase signal
  //
  Status = PeiServicesInstallPpi (&gEndOfPeiSignalPpi);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%a: Fail to signal End of PEI event.", __FUNCTION__));
    ASSERT(FALSE);
  }
  //
  // Update the contents of BSP stack HOB to reflect the real stack info passed to DxeCore.
  //
  UpdateStackHob ((EFI_PHYSICAL_ADDRESS)(UINTN) BaseOfStack, STACK_SIZE);

  DEBUG ((DEBUG_INFO, "DXE Core new stack at %x, stack pointer at %x\n", BaseOfStack, TopOfStack));

  //
  // Transfer the control to the entry point of DxeCore.
  //
  RiscVPlatformHandOffToDxeCore (BaseOfStack, TopOfStack, DxeCoreEntryPoint, HobList);
}
