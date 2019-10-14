/** @file
  RISC-V CPU library definitions.

  Copyright (c) 2016 - 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef RISCV_CPU_LIB_H_
#define RISCV_CPU_LIB_H_

#include "RiscVImpl.h"

/**
  RISCV_TRAP_HANDLER
**/
typedef
VOID
(EFIAPI *RISCV_TRAP_HANDLER)(
  VOID
  );

VOID
RiscVSetScratch (RISCV_MACHINE_MODE_CONTEXT *RiscvContext);

UINT32
RiscVGetScratch (VOID);

UINT32
RiscVGetTrapCause (VOID);

UINT64
RiscVReadMachineTimer (VOID);

VOID
RiscVSetMachineTimerCmp (UINT64);

UINT64
RiscVReadMachineTimerCmp(VOID);

UINT64
RiscVReadMachineIE(VOID);

UINT64
RiscVReadMachineIP(VOID);

UINT64
RiscVReadMachineStatus(VOID);

VOID
RiscVWriteMachineStatus(UINT64);

UINT64
RiscVReadMachineTvec(VOID);

UINT64
RiscVReadMisa (VOID);

UINT64
RiscVReadMVendorId (VOID);

UINT64
RiscVReadMArchId (VOID);

UINT64
RiscVReadMImplId (VOID);

#endif
