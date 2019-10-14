/** @file
  RISC-V OpesbSBI header file reference.

  Copyright (c) 2019, Hewlett Packard Enterprise Development LP. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef EDK2_SBI_TYPES_H_
#define EDK2_SBI_TYPES_H_

typedef INT8    s8;
typedef UINT8   u8;
typedef UINT8   uint8_t;

typedef INT16   s16;
typedef UINT16  u16;
typedef INT16   int16_t;
typedef UINT16  uint16_t;

typedef INT32   s32;
typedef UINT32  u32;
typedef INT32   int32_t;
typedef UINT32  uint32_t;

typedef INT64   s64;
typedef UINT64  u64;
typedef INT64   int64_t;
typedef UINT64  uint64_t;

#define PRILX   "016lx"

typedef INT32    bool;
typedef unsigned long  ulong;
typedef UINT64   uintptr_t;
typedef UINT64   size_t;
typedef INT64    ssize_t;
typedef UINT64   virtual_addr_t;
typedef UINT64   virtual_size_t;
typedef UINT64   physical_addr_t;
typedef UINT64   physical_size_t;

#define __packed        __attribute__((packed))
#define __noreturn      __attribute__((noreturn))
#endif
