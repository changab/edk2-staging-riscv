//------------------------------------------------------------------------------
//
// RISC-V CPU functions.
//
// Copyright (c) 2016 - 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//
//------------------------------------------------------------------------------
#include <Base.h>
#include <RiscVImpl.h>

.data

.text
.align 3

.global ASM_PFX(RiscVSetScratch)
.global ASM_PFX(RiscVGetScratch)
.global ASM_PFX(RiscVGetMachineTrapCause)
.global ASM_PFX(RiscVReadMachineIE)
.global ASM_PFX(RiscVReadMachineIP)
.global ASM_PFX(RiscVReadMachineStatus)
.global ASM_PFX(RiscVWriteMachineStatus)
.global ASM_PFX(RiscVReadMachineTvec)
.global ASM_PFX(RiscVReadMisa)
.global ASM_PFX(RiscVReadMVendorId)
.global ASM_PFX(RiscVReadMArchId)
.global ASM_PFX(RiscVReadMImplId)
//
// Set machine mode scratch.
// @param a0 : Pointer to RISCV_MACHINE_MODE_CONTEXT.
//
ASM_PFX (RiscVSetScratch):
    csrrw a1, RISCV_CSR_MACHINE_MSCRATCH, a0
    ret

//
// Get machine mode scratch.
// @retval a0 : Pointer to RISCV_MACHINE_MODE_CONTEXT.
//
ASM_PFX (RiscVGetScratch):
    csrrs a0, RISCV_CSR_MACHINE_MSCRATCH, 0
    ret

//
// Get machine trap cause CSR.
//
ASM_PFX (RiscVGetMachineTrapCause):
    csrrs a0, RISCV_CSR_MACHINE_MCAUSE, 0
    ret

//
// Get machine interrupt enable
//
ASM_PFX (RiscVReadMachineIE):
    csrr a0, RISCV_CSR_MACHINE_MIE
    ret

//
// Get machine interrupt pending
//
ASM_PFX (RiscVReadMachineIP):
    csrr a0, RISCV_CSR_MACHINE_MIP
    ret

//
// Get machine status
//
ASM_PFX(RiscVReadMachineStatus):
    csrr a0, RISCV_CSR_MACHINE_MSTATUS
    ret

//
// Set machine status
//
ASM_PFX(RiscVWriteMachineStatus):
    csrw RISCV_CSR_MACHINE_MSTATUS, a0
    ret

//
// Get machine trap vector
//
ASM_PFX(RiscVReadMachineTvec):
    csrr a0, RISCV_CSR_MACHINE_MTVEC
    ret

//
// Read machine ISA
//
ASM_PFX(RiscVReadMisa):
    csrr a0, RISCV_CSR_MACHINE_MISA
    ret

//
// Read machine vendor ID
//
ASM_PFX(RiscVReadMVendorId):
    csrr a0, RISCV_CSR_MACHINE_MVENDORID
    ret

//
// Read machine architecture ID
//
ASM_PFX(RiscVReadMArchId):
    csrr a0, RISCV_CSR_MACHINE_MARCHID
    ret

//
// Read machine implementation ID
//
ASM_PFX(RiscVReadMImplId):
    csrr a0, RISCV_CSR_MACHINE_MIMPID
    ret

