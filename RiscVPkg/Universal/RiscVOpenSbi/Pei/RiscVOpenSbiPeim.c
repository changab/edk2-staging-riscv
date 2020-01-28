/** @file
The module to produce OpenSBI PPI.

Copyright (c) 2020, Hewlett Packard Development LP. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/PeiServicesLib.h>
#include <IndustryStandard/RiscVOpensbi.h>
#include <Ppi/RiscVOpenSbiPeim.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_types.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_init.h>

EFI_STATUS
EFIAPI
PeiOpenSbiGetSpecVersion (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *SpecVersion
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiGetImplId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplId
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiGetImplVersion (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplVersion
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiProbeExtension (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  IN  INTN                         ExtensionId
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiGetMvendorId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MvendorId
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiGetMarchId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MarchId
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiGetMimpId (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *Mimpid
  )
{
    // TODO
    return EFI_SUCCESS;
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
  Clear pending timer interrupt bit and set clock for next event after StimeValue time.

  To clear the timer without scheduling a timer event, set StimeValue to a practically infinite value.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiSetTimer (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN UINT64                         StimeValue
  )
{
  sbi_set_timer(StimeValue);
  return EFI_SUCCESS;
}

/**
  Write char to debug console
  Will block if characters are pending to be transmitted or the receiving terminal is not ready to receive a byte.
  If the console, does not exist, nothing happens.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiConsolePutChar (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN INTN                           ConsoleChar
  )
{
  sbi_console_putchar(ConsoleChar);
  return EFI_SUCCESS;
}

/**
  Read single byte from debug console.

  Doesn't block

  Return values
  EFI_SUCCESS
  EFI_LOAD_ERROR -> Doesn't write to ConsoleChar
**/
EFI_STATUS
EFIAPI
PeiOpenSbiConsoleGetChar (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  OUT INTN                          *ConsoleChar
  )
{
  int ReadChar = sbi_set_timer(ConsoleChar);
  if (ReadChar == -1) {
    return EFI_LOAD_ERROR;
  }

  *ConsoleChar = ReadChar;
  return EFI_SUCCESS;
}

/**
  Clear all pending IPIs for the invoking hart.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiClearIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
  sbi_clear_ipi();
  return EFI_SUCCESS;
}

/**
  Send an inter-processor interrupt to all the harts defined in HartMask.
  Interprocessor interrupts manifest at the receiving harts as Supervisor
  Software Interrupts.

  HartMask is a virtual address that points to a bit-vector of harts. The bit
  vector is represented as a sequence of unsigned longs whose length equals the
  number of harts in the system divided by the number of bits in an unsigned
  long, rounded up to the next integer.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiSendIpi (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  )
{
  sbi_send_ipi(HartMask);
  return EFI_SUCCESS;
}

/**
  Execute FENCE.I instruction on remote hart.

  HartMask is a virtual address that points to a bit-vector of harts. The bit
  vector is represented as a sequence of unsigned longs whose length equals the
  number of harts in the system divided by the number of bits in an unsigned
  long, rounded up to the next integer.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteFenceI (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  )
{
  sbi_remote_fence_i(HartMask);
  return EFI_SUCCESS;
}

/**
  Execute one or more SFENCE.VMA instructions on remote hart, covering the range of virtual addresses between Start and Size.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteSfenceVma (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size
  )
{
  sbi_remote_sfence_vma(HartMask, Start, Size);
  return EFI_SUCCESS;
}

/**
  Same as PeiOpenSbiRemoteSfenceVma but only for the given Asid.
**/
EFI_STATUS
EFIAPI
PeiOpenSbiRemoteSfenceVmaAsid (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size,
  IN  UINTN                          Asid
  )
{
  sbi_remote_sfence_vma_asid(HartMask, Start, Size, Asid);
  return EFI_SUCCESS;
}

/**
  Shuts down all harts. Does not return.
**/
VOID
EFIAPI
PeiOpenSbiShutdown (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
  sbi_shutdown();
  UNREACHABLE();
}

//
// OpenSBI PPI interface function for the legacy extension
//
PEI_RISCV_OPENSBI_LEGACY_PPI mOpenSbiLegacyPpi = {
  0x02,  // TODO: Should be SBI spec version
  PeiOpenSbiSetTimer,
  PeiOpenSbiConsolePutChar,
  PeiOpenSbiConsoleGetChar,
  PeiOpenSbiClearIpi,
  PeiOpenSbiSendIpi,
  PeiOpenSbiRemoteFenceI,
  PeiOpenSbiRemoteSfenceVma,
  PeiOpenSbiRemoteSfenceVmaAsid,
  PeiOpenSbiShutdown
};

EFI_STATUS
EFIAPI
PeiOpenSbiHartStart (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiHartStop (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
    // TODO
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiHartGetStatus (
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  )
{
    // TODO
    return EFI_SUCCESS;
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

EFI_STATUS
EFIAPI
PeiOpenSbiLibraryScratchThisHartPtr (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  OUT struct sbi_scratch            **ScratchPtr
  )
{
    *ScratchPtr = (struct sbi_scratch *)csr_read(CSR_MSCRATCH);
    // TODO: Can this ever fail?
    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiOpenSbiLibraryPlatformPtr (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  IN  struct sbi_scratch             *ScratchPtr,
  OUT CONST struct sbi_platform     **PlatformPtr
  )
{
    if (ScratchPtr == NULL)
        return EFI_INVALID_PARAMETER;

    *PlatformPtr = (CONST struct sbi_platform *)(ScratchPtr->platform_addr);
    // TODO: Do we need this function at all? In OpenSBI it's just a macro.
    return EFI_SUCCESS;
}

typedef struct sbi_scratch *(*h2s)(ulong hartid);

EFI_STATUS
EFIAPI
PeiOpenSbiLibraryHartIdToScratch (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  IN  struct sbi_scratch             *AnyScratchPtr,
  IN  UINTN                           HartId,
  OUT CONST struct sbi_scratch      **ScratchPtr
  )
{
    if (AnyScratchPtr == NULL)
        return EFI_INVALID_PARAMETER;

    *ScratchPtr = ((h2s)AnyScratchPtr->hartid_to_scratch)(HartId);

    // TODO: Maybe check if *ScratchPtr is NULL?
    return EFI_SUCCESS;
}

VOID
EFIAPI
PeiOpenSbiLibrarySbiInit (
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  IN  struct sbi_scratch             *ScratchPtr
  )
{
    sbi_init(ScratchPtr);
    // TOOD: Never returns. Should we put an annotation?
}

//
// OpenSBI PPI interface function for the OpenSBI library itself
// Can only be called from M-Mode!
//
PEI_RISCV_OPENSBI_LIBRARY_PPI mOpenSbiLibraryPpi = {
  0x00,
  PeiOpenSbiLibraryScratchThisHartPtr,
  PeiOpenSbiLibraryPlatformPtr,
  PeiOpenSbiLibraryHartIdToScratch,
  PeiOpenSbiLibrarySbiInit
};

EFI_PEI_PPI_DESCRIPTOR mOpenSbiPpiDesc[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiOpenSbiBasePpiGuid,
    &mOpenSbiBasePpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiOpenSbiLegacyPpiGuid,
    &mOpenSbiLegacyPpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiOpenSbiHsmPpiGuid,
    &mOpenSbiHsmPpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiOpenSbiVendorPpiGuid,
    &mOpenSbiVendorPpi
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gPeiOpenSbiLibraryPpiGuid,
    &mOpenSbiLibraryPpi
  }
};

/**
  Entry point of this module.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @return Status.

**/
EFI_STATUS
EFIAPI
OpenSbiPeimEntry (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS                        Status;

  Status = PeiServicesInstallPpi (mOpenSbiPpiDesc);

  return Status;
}
