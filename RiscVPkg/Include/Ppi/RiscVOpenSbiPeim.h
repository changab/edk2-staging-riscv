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

///
/// SpecVersion:
/// The minor number of the SBI specification is encoded in the low 24 bits,
/// with the major number encoded in the next 7 bits.  Bit 32 must be 0 and is
/// reserved for future expansion.
///
typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_SPEC_VERSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *SpecVersion
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_IMPL_ID)(
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI *This,
  OUT UINTN                      *ImplId
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_IMPLVERSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *ImplVersion
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_PROBE_EXTENSION)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  IN  INTN                         ExtensionId
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_MVENDOR_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MvendorId
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_GET_MARCH_ID)(
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  PEI_RISCV_OPENSBI_BASE_PPI  *This,
  OUT UINTN                       *MarchId
  );

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

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_SET_TIMER)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  UINT64                         StimeValue
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_CONSOLE_PUT_CHAR)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  INTN                           ConsoleChar
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_CONSOLE_GET_CHAR)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  OUT INTN                          *ConsoleChar
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_CLEAR_IPI)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_SEND_IPI)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_REMOTE_FENCE_I)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This,
  IN  CONST UINTN                   *HartMask,
  IN  UINTN                          Start,
  IN  UINTN                          Size
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_REMOTE_SFENCE_VMA_ASID)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI   *This,
  IN  CONST UINTN                    *HartMask,
  IN  UINTN                           Start,
  IN  UINTN                           Size,
  IN  UINTN                           Asid
  );

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

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_START)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_STOP)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_HART_GET_STATUS)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LEGACY_PPI  *This
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

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_LIBRARY_SCRATCH_THIS_HART_PTR)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  OUT struct sbi_scratch            **ScratchPtr
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_LIBRARY_PLATFORM_PTR)(
  IN  EFI_PEI_SERVICES              **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI  *This,
  IN  struct sbi_scratch             *ScratchPtr,
  OUT const struct sbi_platform     **PlatformPtr
  );

typedef
EFI_STATUS
(EFIAPI *PEI_RISCV_OPENSBI_LIBRARY_HART_ID_TO_SCRATCH)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  PEI_RISCV_OPENSBI_LIBRARY_PPI *This,
  IN  struct sbi_scratch            *AnyScratchPtr,
  IN  UINTN                          HartId,
  OUT const struct sbi_scratch     **ScratchPtr
  );

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
