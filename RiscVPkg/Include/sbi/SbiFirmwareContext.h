/** @file
  RISC-V OpesbSBI Platform Firmware context definition

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef SBI_FIRMWARE_CONTEXT_H_
#define SBI_FIRMWARE_CONTEXT_H_

#include <RiscVImpl.h>

#define RISC_V_MAX_HART_SUPPORTED 16

//
// keep the structure member in 64-bit alignment.
//
typedef struct {
    UINT64          IsaExtensionSupported;  // The ISA extension this core supported.
    RISCV_UINT128   MachineVendorId;        // Machine vendor ID
    RISCV_UINT128   MachineArchId;          // Machine Architecture ID
    RISCV_UINT128   MachineImplId;          // Machine Implementation ID
} EFI_RISCV_FIRMWARE_CONTEXT_HART_SPECIFIC;

#define FIRMWARE_CONTEXT_HART_SPECIFIC_SIZE  (64 * 7)

typedef struct {
  VOID            *PeiServiceTable;       // PEI Service table
  EFI_RISCV_FIRMWARE_CONTEXT_HART_SPECIFIC  *HartSpecific[RISC_V_MAX_HART_SUPPORTED];
} EFI_RISCV_OPENSBI_FIRMWARE_CONTEXT;
#endif

