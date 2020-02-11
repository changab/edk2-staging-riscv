/** @file
  Defines the PEI_RISCV_OPENSBI_LEGACY_PPI to let PEIMs call OpenSBI

Copyright (c) 2020, Hewlett Packard Development LP. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _PEI_RISCV_OPENSBI_PPI_H_
#define _PEI_RISCV_OPENSBI_PPI_H_

#include <sbi/sbi_scratch.h>
#include <sbi/sbi_platform.h>

///
/// Global ID for the PEI_RISCV_OPENSBI_BASE_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_BASE_PPI_GUID  \
  { \
    0x9af5b3c3, 0x1e8c, 0x4219, { 0xa4, 0xfb, 0x3f, 0xb8, 0xb2, 0xb8, 0xfe, 0xa1 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_BASE_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_BASE_PPI PEI_RISCV_OPENSBI_BASE_PPI;

/**
  Get the implemented SBI specification version

  The minor number of the SBI specification is encoded in the low 24 bits,
  with the major number encoded in the next 7 bits.  Bit 32 must be 0 and is
  reserved for future expansion.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] SpecVersion          The Version of the SBI specification.
  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_UNSUPPORTED          The operation is unsupported.
  @retval EFI_INVALID_PARAMETER    A parameter is invalid.
  @retval EFI_ACCESS_DENIED        Access was denied.
  @retval EFI_LOAD_EERROR          The given address is invalid.
  @retval EFI_ALREADY_STARTED      The feature is already available.
  @retval other                    Some error occured.
  // TODO: Can it fail?
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_SPEC_VERSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *SpecVersion
  );

/**
  Get the SBI implementation ID

  This ID is used to idenetify a specific SBI implementation in order to work
  around any quirks it might have.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplId               The ID of the SBI implementation.
  @retval EFI_SUCCESS              The operation succeeds.
  // TODO: Can it fail?
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_IMPL_ID)(
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI *This,
  OUT UINTN                      *ImplId
  );

/**
  Get the SBI implementation version

  The version of this SBI implementation.
  The encoding of this number is determined by the specific SBI implementation.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] ImplVersion          The version of the SBI implementation.
  @retval EFI_SUCCESS              The operation succeeds.
  // TODO: Can it fail?
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_IMPLVERSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplVersion
  );

/**
  Probe whether an SBI extension is available

  ProbeResult is set to 0 if the extension is not available or to an extension
  specified value if it is available.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ExtensionId          The extension ID.
  @param[out] ProbeResult          The return value of the probe.
  @retval EFI_SUCCESS              The operation succeeds.
  // TODO: What should be the return values?
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_PROBE_EXTENSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  IN  INTN                         ExtensionId,
  OUT INTN                        *ProbeResult
  );

/**
  Get the CPU's vendor ID

  Reads the mvendorid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MvendorId            The CPU's vendor ID.
  @retval EFI_SUCCESS              The operation succeeds.
  // TODO: Can it fail?
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_MVENDOR_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MvendorId
  );

/**
  Get the CPU's architecture ID

  Reads the marchid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MarchId              The CPU's architecture ID.
  @retval EFI_SUCCESS              The operation succeeds.
  // TODO: Can it fail?
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_MARCH_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MarchId
  );

/**
  Get the CPU's implementation ID

  Reads the mimpid CSR.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] MimpId               The CPU's implementation ID.
  @retval EFI_SUCCESS              The operation succeeds.
  // TODO: Can it fail?
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_MIMP_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *Mimpid
  );

struct _PEI_RISCV_OPENSBI_BASE_PPI {
  UINTN                              Version;
  PEI_RISCV_OPENSBI_GET_SPEC_VERSION       OpenSbiGetSpecVersion;
  PEI_RISCV_OPENSBI_GET_IMPL_ID            OpenSbiGetImplId;
  PEI_RISCV_OPENSBI_GET_IMPLVERSION        OpenSbiGetImplVersion;
  PEI_RISCV_OPENSBI_PROBE_EXTENSION        OpenSbiProbleExtension;
  PEI_RISCV_OPENSBI_GET_MVENDOR_ID         OpenSbiGetMvendorId;
  PEI_RISCV_OPENSBI_GET_MARCH_ID           OpenSbiGetMarchId;
  PEI_RISCV_OPENSBI_GET_MIMP_ID            OpenSbiGetMimpId;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_LEGACY_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_LEGACY_PPI_GUID  \
  { \
    0xc80a737c, 0x566b, 0x4709, { 0x8f, 0x74, 0x58, 0x30, 0xee, 0x2a, 0xd7, 0x36 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_LEGACY_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_LEGACY_PPI PEI_RISCV_OPENSBI_LEGACY_PPI;

/**
  Clear pending timer interrupt bit and set timer for next event after StimeValue.

  To clear the timer without scheduling a timer event, set StimeValue to a practically infinite value.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StimeValue           The time offset to the next scheduled timer interrupt.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_SET_TIMER)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  UINT64                         StimeValue
  );

/**
  Write a byte to the system's console.

  Will block if characters are pending to be transmitted or the receiving
  terminal is not ready to receive a byte. If the console, does not exist,
  the byte is discarded.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The time offset to the next scheduled timer interrupt.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_CONSOLE_PUT_CHAR)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  INTN                           ConsoleChar
  );

/**
  Read a byte from the system's console.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ConsoleChar          The pointer in which the read byte is written.
  @retval EFI_SUCCESS              The operation succeeds.
  // TODO: Add failure code
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_CONSOLE_GET_CHAR)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  OUT INTN                          *ConsoleChar
  );

/**
  Clear pending IPIs for the calling hart.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_CLEAR_IPI)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

/**
  Send IPI to all harts specified in the mask.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_SEND_IPI)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  );

/**
  Instructs remote harts to execute a FENCE.I instruction. 

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_REMOTE_FENCE_I)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  );

/**
  Instructs remote harts to execute one or more SFENCE.VMA instructions over a range.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  HartMask             Virtual address pointing to the hart mask,
                                   which is an array of `unsigned long`s.
  @param[in]  Start                The address of where the range begins.
  @param[in]  Size                 The total size of the range.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size
  );

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
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA_ASID)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI   *This,
  IN  CONST UINTN                    *HartMask,
  IN  UINTN                           Start,
  IN  UINTN                           Size,
  IN  UINTN                           Asid
  );

/**
  Shuts all harts down.

  Does not return.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_SHUTDOWN)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

///
/// This PPI contains a set of services to interact with OpenSBI
///
struct _PEI_RISCV_OPENSBI_LEGACY_PPI {
  UINTN                                    Version;
  PEI_RISCV_OPENSBI_SET_TIMER              OpenSbiSetTimer;
  PEI_RISCV_OPENSBI_CONSOLE_PUT_CHAR       OpenSbiConsolePutChar;
  PEI_RISCV_OPENSBI_CONSOLE_GET_CHAR       OpenSbiConsoleGetChar;
  PEI_RISCV_OPENSBI_CLEAR_IPI              OpenSbiClearIpi;
  PEI_RISCV_OPENSBI_SEND_IPI               OpenSbiSendIpi;
  PEI_RISCV_OPENSBI_REMOTE_FENCE_I         OpenSbiRemoteFenceI;
  PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA      OpenSbiRemoteSfenceVma;
  PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA_ASID OpenSbiRemoteSfenceVmaAsid;
  PEI_RISCV_OPENSBI_SHUTDOWN               OpenSbiShutdown;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_HSM_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_HSM_PPI_GUID  \
  { \
    0x13421726, 0xa40c, 0x479c, { 0x94, 0xd2, 0x8f, 0xf2, 0x97, 0x32, 0xfb, 0xb4 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_HSM_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_HSM_PPI PEI_RISCV_OPENSBI_HSM_PPI;

/**
  Politely ask the SBI to start a given hart.

  This call may return before the hart has actually started executing, if the
  SBI implementation can guarantee that the hart is actually going to start.

  Before the hart jumps to StartAddr, the hart MUST configure PMP if present
  and switch to S-mode.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  StartAddr            The physical address, where the hart starts
                                   executing from.
  @param[in]  Priv                 An XLEN-bit value, which will be in register
                                   a2 when the hart starts.
  // TODO: Define error codes
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_START)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  IN  UINTN                          HartId,
  IN  UINTN                          StartAddr,
  IN  UINTN                          Priv,
  );

/**
  Return execution of the calling hart to SBI.

  MUST be called in S-Mode with user interrupts disabled.
  This call is not expected to return, unless a failure occurs.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  // TODO: Define error codes
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_STOP)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

/**
  Get the current status of a hart.

  Since harts can transition between states at any time, the status retrieved
  by this function may already be out of date, once it completes execution.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[out] HartStatus           The pointer in which the hart's status is
                                   stored.
  @retval EFI_SUCCESS              The operation succeeds.
  @retval EFI_INVALID_PARAMETER    A parameter is invalid.
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_GET_STATUS)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  OUT UINTN                         *HartStatus
  );

///
/// Hart State Management Extension
///
struct _PEI_RISCV_OPENSBI_HSM_PPI {
  UINTN                                    Version;
  PEI_RISCV_OPENSBI_HART_START             OpenSbiHartStart;
  PEI_RISCV_OPENSBI_HART_STOP              OpenSbiHartStop;
  PEI_RISCV_OPENSBI_HART_GET_STATUS        OpenSbiHartGetStatus;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_VENDOR_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_VENDOR_PPI_GUID  \
  { \
    0x44ff1fcf, 0x1d04, 0x4fba, { 0x8e, 0x44, 0x5f, 0x68, 0xfe, 0x07, 0xcd, 0xe4 } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_VENDOR_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_VENDOR_PPI PEI_RISCV_OPENSBI_VENDOR_PPI;

/**
  TODO

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ExtensionId          TODO
  @retval TODO
**/
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_VENDOR_CALL)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_VENDOR_PPI  *This,
  IN  UINTN                          ExtensionId,
  ...
  );

///
/// Vendor Specific extension space: Extension Ids 0x09000000 through 0x09FFFFFF
///
struct _PEI_RISCV_OPENSBI_VENDOR_PPI {
  UINTN                         Version;
  PEI_RISCV_OPENSBI_VENDOR_CALL OpenSbiVendorCall;
};

///
/// Global ID for the PEI_RISCV_OPENSBI_LIBRARY_PPI_GUID.
///
#define PEI_RISCV_OPENSBI_LIBRARY_PPI_GUID  \
  { \
    0x34922165, 0xa5ca, 0x470f, { 0x83, 0x2f, 0x14, 0xc9, 0x42, 0xfa, 0x83, 0xdf  } \
  }

///
/// Forward declaration for PEI_RISCV_OPENSBI_LIBRARY_PPI.
///
typedef struct _PEI_RISCV_OPENSBI_LIBRARY_PPI PEI_RISCV_OPENSBI_LIBRARY_PPI;

// TODO: Likely I will move this to a header
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_LIBRARY_SCRATCH_THIS_HART_PTR)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  OUT struct sbi_scratch            **ScratchPtr
  );

// TODO: Likely I will move this to a header
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_LIBRARY_PLATFORM_PTR)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  IN  struct sbi_scratch             *ScratchPtr,
  OUT const struct sbi_platform     **PlatformPtr
  );

// TODO: Likely I will move this to a header
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_LIBRARY_HART_ID_TO_SCRATCH)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI *This,
  IN  struct sbi_scratch            *AnyScratchPtr,
  IN  UINTN                          HartId,
  OUT const struct sbi_scratch     **ScratchPtr
  );

/**
  Initialize the OpenSBI library for the current hart and jump to the next
  booting stage.

  The following prerequisites MUST be met:
  1. The 'mscratch' CSR is pointing to sbi_scratch of current HART
  2. Stack pointer (SP) is setup for current HART
  3. Interrupts are disabled in MSTATUS CSR
  4. All interrupts are disabled in MIE CSR

  This call does not return.

  @param[in]  PeiServices          The PEI Services pointer.
  @param[in]  This                 The PPI instance pointer.
  @param[in]  ScratchPtr           The pointer to the current hart's scratch
                                   space.
**/
typedef
VOID
(EFIAPI *PEI_RISCV_OPENSBI_LIBRARY_SBI_INIT)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  IN  struct sbi_scratch             *ScratchPtr
  );

///
/// Library API
///
struct _PEI_RISCV_OPENSBI_LIBRARY_PPI {
  UINTN                                           Version;
  PEI_RISCV_OPENSBI_LIBRARY_SCRATCH_THIS_HART_PTR OpenSbiLibraryScratchThisHartPtr;
  PEI_RISCV_OPENSBI_LIBRARY_PLATFORM_PTR          OpenSbiLibraryPlatformPtr;
  PEI_RISCV_OPENSBI_LIBRARY_HART_ID_TO_SCRATCH    OpenSbiLibraryHartIdToScratch;
  PEI_RISCV_OPENSBI_LIBRARY_SBI_INIT              OpenSbiLibrarySbiInit;
};

#endif
