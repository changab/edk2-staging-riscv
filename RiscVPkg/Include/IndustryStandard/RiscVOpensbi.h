/** @file
  SBI inline function calls.

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef EDK2_SBI_H_
#define EDK2_SBI_H_

#include <include/sbi/riscv_asm.h> // Reference to header file in opensbi
#include <RiscVImpl.h>
#include <sbi/sbi_types.h>  // Reference to header file wrapper

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2
#define SBI_CLEAR_IPI 3
#define SBI_SEND_IPI 4
#define SBI_REMOTE_FENCE_I 5
#define SBI_REMOTE_SFENCE_VMA 6
#define SBI_REMOTE_SFENCE_VMA_ASID 7
#define SBI_SHUTDOWN 8

#define SBI_CALL(which, arg0, arg1, arg2) ({ \
    register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0); \
    register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1); \
    register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2); \
    register uintptr_t a7 asm ("a7") = (uintptr_t)(which); \
    asm volatile ("ecall" \
         : "+r" (a0) \
         : "r" (a1), "r" (a2), "r" (a7) \
         : "memory"); \
        a0; \
})

#define SBI_CALL_0(which) SBI_CALL(which, 0, 0, 0)
#define SBI_CALL_1(which, arg0) SBI_CALL(which, arg0, 0, 0)
#define SBI_CALL_2(which, arg0, arg1) SBI_CALL(which, arg0, arg1, 0)

#define sbi_console_putchar(ch)    SBI_CALL_1(SBI_CONSOLE_PUTCHAR, ch)
#define sbi_console_getchar()      SBI_CALL_0(SBI_CONSOLE_GETCHAR)
#define sbi_set_timer(stime_value) SBI_CALL_1(SBI_SET_TIMER, stime_value)
#define sbi_shutdown()             SBI_CALL_0(SBI_SHUTDOWN)
#define sbi_clear_ipi()            SBI_CALL_0(SBI_CLEAR_IPI)
#define sbi_send_ipi(hart_mask)       SBI_CALL_1(SBI_SEND_IPI, hart_mask)
#define sbi_remote_fence_i(hart_mask) SBI_CALL_1(SBI_REMOTE_FENCE_I, hart_mask)
#define sbi_remote_sfence_vma(hart_mask, start, size) SBI_CALL_1(SBI_REMOTE_SFENCE_VMA, hart_mask)
#define sbi_remote_sfence_vma_asid(hart_mask, start, size, asid) SBI_CALL_1(SBI_REMOTE_SFENCE_VMA_ASID, hart_mask)

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
