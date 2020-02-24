/** @file
The module to produce OpenSBI PPI.

Copyright (c) 2020, Hewlett Packard Development LP. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <IndustryStandard/RiscV.h>
#include <IndustryStandard/RiscVOpensbi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/RiscVOpenSbiPpi.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_types.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_init.h>

VOID
CallLegacyEcallHandler(
);

EFI_STATUS
EFIAPI
CallEcallHandler(
  IN  UINTN  ExtensionId,
  IN  UINTN  FunctionId,
  IN  UINTN  Arg0,
  IN  UINTN  Arg1,
  IN  UINTN  Arg2,
  IN  UINTN  Arg3,
  IN  UINTN  Arg4,
  IN  UINTN  Arg5,
  OUT UINTN *OutVal
  )
{
  EFI_STATUS  Status;
  INTN        ret;
  UINTN       hartid = sbi_current_hartid();
  // Or RISCV_CAUSE_MACHINE_ECALL?
  UINTN       mcause = RISCV_CAUSE_SUPERVISOR_ECALL;
  // TODO: Check whether we can use this scratch space
  UINTN       scratch = csr_read(CSR_MSCRATCH);

  struct sbi_trap_regs regs;
  SetMem(&regs, sizeof (struct sbi_trap_regs), 0);
	regs.a0 = Arg0;
	regs.a1 = Arg1;
	regs.a2 = Arg2;
	regs.a3 = Arg3;
	regs.a4 = Arg4;
	regs.a5 = Arg5;
	regs.a6 = FunctionId;
	regs.a7 = ExtensionId;
  // I think we can ignore the other registers
  // Only mepc is used ... TODO

  ret = sbi_ecall_handler(hartid, mcause, &regs, (struct sbi_scratch *)scratch);

  if (ret != 0) {
    return -1;// TODOEFI_FAILURE;
  }

  switch (regs.a0) {
    case SBI_SUCCESS:
      if (OutVal != NULL) {
        *OutVal = (UINTN) regs.a1;
      }
      Status  = EFI_SUCCESS;
    case SBI_ERR_FAILED:
      Status = EFI_DEVICE_ERROR;
      break;
    case SBI_ERR_NOT_SUPPORTED:
      Status = EFI_UNSUPPORTED;
      break;
    case SBI_ERR_INVALID_PARAM:
      Status = EFI_INVALID_PARAMETER;
      break;
    case SBI_ERR_DENIED:
      Status = EFI_ACCESS_DENIED;
      break;
    case SBI_ERR_INVALID_ADDRESS:
      Status = EFI_LOAD_ERROR;
      break;
    case SBI_ERR_ALREADY_AVAILABLE:
      Status = EFI_ALREADY_STARTED;
      break;
  }

  return Status;
}

/**
  Get the implemented SBI specification version

  The minor number of the SBI specification is encoded in the low 24 bits,
  with the major number encoded in the next 7 bits.  Bit 32 must be 0 and is
  reserved for future expansion.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] SpecVersion          The Version of the SBI specification.
**/
VOID
EFIAPI
PeiOpenSbiGetSpecVersion (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *SpecVersion
  )
{
  CallEcallHandler(SBI_BASE_EXT, SBI_GET_SPEC_VERSION_FUNC, 0, 0, 0, 0, 0, 0, SpecVersion);
}

/**
  Get the SBI implementation ID

  This ID is used to idenetify a specific SBI implementation in order to work
  around any quirks it might have.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplId               The ID of the SBI implementation.
**/
VOID
EFIAPI
PeiOpenSbiGetImplId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplId
  )
{
  CallEcallHandler(SBI_BASE_EXT, SBI_GET_IMPL_ID_FUNC, 0, 0, 0, 0, 0, 0, ImplId);
}

/**
  Get the SBI implementation version

  The version of this SBI implementation.
  The encoding of this number is determined by the specific SBI implementation.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplVersion          The version of the SBI implementation.
**/
VOID
EFIAPI
PeiOpenSbiGetImplVersion (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplVersion
  )
{
  CallEcallHandler(SBI_BASE_EXT, SBI_GET_IMPL_VERSION_FUNC, 0, 0, 0, 0, 0, 0, ImplVersion);
}

/**
  Probe whether an SBI extension is available

  ProbeResult is set to 0 if the extension is not available or to an extension
  specified value if it is available.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ExtensionId          The extension ID.
  @param[out] ProbeResult          The return value of the probe.
**/
VOID
EFIAPI
PeiOpenSbiProbeExtension (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  IN  INTN                         ExtensionId,
  OUT INTN                        *ProbeResult
  )
{
  CallEcallHandler(SBI_BASE_EXT, SBI_PROBE_EXTENSION_FUNC, 0, 0, 0, 0, 0, 0, (UINTN *)ProbeResult);
}

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MvendorId            The CPU's vendor ID.
**/
VOID
EFIAPI
PeiOpenSbiGetMvendorId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MvendorId
  )
{
  CallEcallHandler(SBI_BASE_EXT, SBI_GET_MVENDORID_FUNC, 0, 0, 0, 0, 0, 0, MvendorId);
}

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MvendorId            The CPU's vendor ID.
**/
VOID
EFIAPI
PeiOpenSbiGetMarchId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MarchId
  )
{
  CallEcallHandler(SBI_BASE_EXT, SBI_GET_MARCHID_FUNC, 0, 0, 0, 0, 0, 0, MarchId);
}

/**
  Get the CPU's architecture ID

  Reads the marchid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MarchId              The CPU's architecture ID.
**/
VOID
EFIAPI
PeiOpenSbiGetMimpId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MimpId
  )
{
  CallEcallHandler(SBI_BASE_EXT, SBI_GET_MIMPID_FUNC, 0, 0, 0, 0, 0, 0, MimpId);
}

//
// OpenSBI PPI interface function for the base extension
//
PEI_RISCV_OPENSBI_BASE_PPI mOpenSbiBasePpi = {
  0x02,  // TODO: Should be SBI spec version
  PeiOpenSbiGetSpecVersion,
  PeiOpenSbiGetImplId,
  PeiOpenSbiGetImplVersion,
  PeiOpenSbiProbeExtension,
  PeiOpenSbiGetMvendorId,
  PeiOpenSbiGetMarchId,
  PeiOpenSbiGetMimpId
};

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

  To clear the timer without scheduling a timer event, set StimeValue to a practically infinite value.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
PeiOpenSbiLegacySetTimer (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN UINT64                         StimeValue
  )
{
  // TODO: return CallEcallHandler(SBI_SET_TIMER_EXT, 0, StimeValue, 0, 0, 0, 0, 0, NULL);
}

/**
  Write a byte to the system's console.

  Will block if characters are pending to be transmitted or the receiving
  terminal is not ready to receive a byte. If the console, does not exist,
  the byte is discarded.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
PeiOpenSbiLegacyConsolePutChar (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN INTN                           ConsoleChar
  )
{
  // TODO: return CallEcallHandler(SBI_CONSOLE_PUTCHAR_EXT, 0, StimeValue, 0, 0, 0, 0, 0, NULL);
}

/**
  Read a byte from the system's console.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The pointer in which the read byte is written.
**/
VOID
EFIAPI
PeiOpenSbiLegacyConsoleGetChar (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  OUT INTN                          *ConsoleChar
  )
{
  // TODO
}

/**
  Clear pending IPIs for the calling hart.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
VOID
EFIAPI
PeiOpenSbiLegacyClearIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
  // TODO
}

/**
  Send IPI to all harts specified in the mask.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
VOID
EFIAPI
PeiOpenSbiLegacySendIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  )
{
  // TODO
}

/**
  Instructs remote harts to execute a FENCE.I instruction. 

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
VOID
EFIAPI
PeiOpenSbiLegacyRemoteFenceI (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  )
{
  // TODO
}

/**
  Instructs remote harts to execute one or more SFENCE.VMA instructions over a range.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
  @param[in]  Start                The address of where the range begins.
  @param[in]  Size                 The total size of the range.
**/
VOID
EFIAPI
PeiOpenSbiLegacyRemoteSfenceVma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size
  )
{
  // TODO
}

/**
  Instructs remote harts to execute one or more SFENCE.VMA instructions over a range, limited to an ASID.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
  @param[in]  Start                The address of where the range begins.
  @param[in]  Size                 The total size of the range.
  @param[in]  Asid                 The ASID to limit the instruction to.
**/
VOID
EFIAPI
PeiOpenSbiLegacyRemoteSfenceVmaAsid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  // TODO
}

/**
  Shuts all harts down.

  Does not return.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
VOID
EFIAPI
PeiOpenSbiLegacyShutdown (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
  // TODO
}

//
// OpenSBI PPI interface function for the legacy extension
//
PEI_RISCV_OPENSBI_LEGACY_PPI mOpenSbiLegacyPpi = {
  0x02,  // TODO: Should be SBI spec version
  PeiOpenSbiLegacySetTimer,
  PeiOpenSbiLegacyConsolePutChar,
  PeiOpenSbiLegacyConsoleGetChar,
  PeiOpenSbiLegacyClearIpi,
  PeiOpenSbiLegacySendIpi,
  PeiOpenSbiLegacyRemoteFenceI,
  PeiOpenSbiLegacyRemoteSfenceVma,
  PeiOpenSbiLegacyRemoteSfenceVmaAsid,
  PeiOpenSbiLegacyShutdown
};

/**
  Politely ask the SBI to start a given hart.

  This call may return before the hart has actually started executing, if the
  SBI implementation can guarantee that the hart is actually going to start.

  Before the hart jumps to StartAddr, the hart MUST configure PMP if present
  and switch to S-mode.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
	@param[in]  HartId               
  @param[in]  StartAddr            The physical address, where the hart starts
                                   executing from.
  @param[in]  Priv                 An XLEN-bit value, which will be in register
                                   a1 when the hart starts.
  @retval EFI_SUCCESS              Hart was stopped and will start executing from StartAddr.
  @retval EFI_LOAD_ERROR           StartAddr is not valid, possibly due to following reasons:
                                   - It is not a valid physical address.
																	 - The address is prohibited by PMP to run in
																	   supervisor mode.
	@retval EFI_INVALID_PARAMETER    HartId is not a valid hart id
	@retval EFI_ALREADY_STARTED      The hart is already running.
	@retval other                    The start request failed for unknown reasons.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiHartStart (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This,
  IN  UINTN                          HartId,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Priv
  )
{
  return CallEcallHandler(SBI_HSM_EXT, SBI_HART_START_FUNC, HartId, StartAddr, Priv, 0, 0, 0, NULL);
}

/**
  Return execution of the calling hart to SBI.

  MUST be called in S-Mode with user interrupts disabled.
  This call is not expected to return, unless a failure occurs.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @retval     EFI_SUCCESS          Never occurs. When successful, the call does not return.
  @retval     other                Failed to stop hard for an unknown reason.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiHartStop (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This
  )
{
  return CallEcallHandler(SBI_HSM_EXT, SBI_HART_STOP_FUNC, 0, 0, 0, 0, 0, 0, NULL);
}

/**
  Get the current status of a hart.

  Since harts can transition between states at any time, the status retrieved
  by this function may already be out of date, once it returns.

  Possible values for HartStatus are:
  0: STARTED
	1: STOPPED
  2: START_REQUEST_PENDING
  3: STOP_REQUEST_PENDING

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] HartStatus           The pointer in which the hart's status is
                                   stored.
  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    A parameter is invalid.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiHartGetStatus (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_HSM_PPI     *This,
  IN  UINTN                          HartId,
  OUT UINTN                         *HartStatus
  )
{
  return CallEcallHandler(SBI_HSM_EXT, SBI_HART_GET_STATUS_FUNC, HartId, 0, 0, 0, 0, 0, HartStatus);
}

//
// OpenSBI PPI interface function for the hart state management extension
//
PEI_RISCV_OPENSBI_HSM_PPI mOpenSbiHsmPpi = {
  0x02,  // TODO: Should be SBI spec version
  PeiOpenSbiHartStart,
  PeiOpenSbiHartStop,
  PeiOpenSbiHartGetStatus
};

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

	To clear the timer without scheduling a timer event, set StimeValue to a
	practically infinite value or mask the timer interrupt by clearing sie.STIE.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
VOID
EFIAPI
PeiOpenSbiSetTimer (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_TIME_PPI    *This,
  IN  UINT64                         StimeValue
  )
{
  CallEcallHandler(SBI_TIME_EXT, 0, StimeValue, 0, 0, 0, 0, 0, NULL);
}

PEI_RISCV_OPENSBI_TIME_PPI mOpenSbiTimePpi = {
  0x02,
  PeiOpenSbiSetTimer
};

EFI_STATUS
EFIAPI
PeiOpenSbiSendIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_IPI_PPI     *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMaskBase
  )
{
  return CallEcallHandler(SBI_IPI_EXT, 0, (UINTN) HartMask, HartMaskBase, 0, 0, 0, 0, NULL);
}

PEI_RISCV_OPENSBI_IPI_PPI mOpenSbiIpiPpi = {
  0x02,
  PeiOpenSbiSendIpi
};

EFI_STATUS
EFIAPI
PeiOpenSbiRemoteFenceI (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMarkBase
  )
{
  return CallEcallHandler(SBI_RFNC_EXT, 0, (UINTN) HartMask, HartMarkBase, 0, 0, 0, 0, NULL);
}

EFI_STATUS
EFIAPI
PeiOpenSbiRemoteSfenceVma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMarkBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  return CallEcallHandler(SBI_RFNC_EXT, 1, (UINTN) HartMask, HartMarkBase, StartAddr, Size, 0, 0, NULL);
}

EFI_STATUS
EFIAPI
PeiOpenSbiRemoteSfenceVmaAsid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMarkBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  return CallEcallHandler(SBI_RFNC_EXT, 2, (UINTN) HartMask, HartMarkBase, StartAddr, Size, Asid, 0, NULL);
}

EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceGvmaVmid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMarkBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Vmid
  )
{
  return CallEcallHandler(SBI_RFNC_EXT, 3, (UINTN) HartMask, HartMarkBase, StartAddr, Size, Vmid, 0, NULL);
}

EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceGvma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMarkBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  return CallEcallHandler(SBI_RFNC_EXT, 3, (UINTN) HartMask, HartMarkBase, StartAddr, Size, 0, 0, NULL);
}

EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceVvmaAsid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMarkBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  return CallEcallHandler(SBI_RFNC_EXT, 3, (UINTN) HartMask, HartMarkBase, StartAddr, Size, Asid, 0, NULL);
}

EFI_STATUS
EFIAPI
PeiOpenSbiRemoteHFenceVvma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_RFNC_PPI    *This,
  IN  UINTN                         *HartMask,
  IN  UINTN                          HartMarkBase,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Size
  )
{
  return CallEcallHandler(SBI_RFNC_EXT, 3, (UINTN) HartMask, HartMarkBase, StartAddr, Size, 0, 0, NULL);
}

PEI_RISCV_OPENSBI_RFNC_PPI mOpenSbiRfncPpi = {
  0x02,
  PeiOpenSbiRemoteFenceI,
  PeiOpenSbiRemoteSfenceVma,
  PeiOpenSbiRemoteSfenceVmaAsid,
  PeiOpenSbiRemoteHFenceGvmaVmid,
  PeiOpenSbiRemoteHFenceGvma,
  PeiOpenSbiRemoteHFenceVvmaAsid,
  PeiOpenSbiRemoteHFenceVvma
};

EFI_STATUS
EFIAPI
PeiOpenSbiVendorCall (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_VENDOR_PPI  *This,
  IN  UINTN                          ExtensionId,
  ...
  )
{
    VA_LIST Args;
    VA_START(Args, ExtensionId);

    // TODO: Call SBI with all provided arguments

    VA_END(Args);
    return EFI_SUCCESS;
}

//
// OpenSBI PPI interface function for the vendor extension
//
PEI_RISCV_OPENSBI_VENDOR_PPI mOpenSbiVendorPpi = {
  0x00,
  PeiOpenSbiVendorCall
};

//
// OpenSBI PPI interface function for the OpenSBI library itself
// Can only be called from M-Mode!
//
PEI_RISCV_OPENSBI_LIBRARY_PPI mOpenSbiLibraryPpi = {
  // TODO: Actually I don't think we need this PPI
  0x00
};
