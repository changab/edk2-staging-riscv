This branch is used to contribute RISC-V architecture to EDK2

The branch owner:
Abner Chang < abner.chang@hpe.com >
Gilbert Chen < gilbert.chen@hpe.com >

## RISC-V EDK2 Port Introduction
RISC-V is an open ISA which was designed to support research and education of
computer architecture, but now it becomes a standard open Instruction Set
Architecture for industry implementations. The RISC-V edk2 project is to create
a new processor binding in UEFI spec and have the RISC-V edk2 implementation.
The goal is to have RISC-V edk2 port as the firmware reference for RISC-V
platforms.

This branch (RISC-V-V2) on edk2-staging is RISC-V edk2 port with RISC-V
OpenSbi (https://github.com/riscv/opensbi) library integrated.
RiscVPkg provides the generic and common modules of RISC-V prcessor.
Two edk2 RISC-V platforms are introduced in this branch,
- SiFive Freedome U500 platform whcih is maintained in FreedomU500VC707Board
under Platform/SiFive/U5SeriesPkg.
- SiFive Freedome U540 HiFive Unleashed platform whcih is maintained in
FreedomU540HiFiveUnleashedBoard under Platform/SiFive/U5SeriesPkg.

Refer to Platform/SiFive/U5Series/Readme.md on edk2-platform repository.

## RISC-V EDK2 Package
```
RiscVPkg       - RISC-V processor package. This package provides RISC-V
                 processor related protocols/libraries accroding to UEFI
                 specification and edk2 implementations.
               - Currently we have two sbi wrapper of header files under
                 RiscVPkg/Include/sbi,
                 One is sbi_bits.h in whcih we fix the duplicate definitons
                 of MAX and MIN.
                 Another one is sbi_types.h which is the binding file for
                 leverage opensbi on edk2 firmware framework.
                 We will work with opensbi open source community to fix the
                 issues we met on edk2 with opensbi in order to remove these
                 two files from edk2 RISC-V port.

RiscVPlatformPkg  - RISC-V platform package. This package provides RISC-V
                    platform common modules, libraries, PCDs and definitoins.
```
## Toolchain of RISC-V EDK2 port
https://github.com/riscv/riscv-gnu-toolchain
You have to clone the toolchain from above link and follow the installation
guidance mentioned in README.md for building RISC-V edk2 port.

This toolchain has been tested on below Linux distribution,
Distributor ID: Ubuntu
Description:    Ubuntu 16.04.6 LTS
Release:        16.04
GCC 9.2.0 is currently supported in RISC-V GNU toolchain.

Build issues may happen on other Linux distributions and have not been discoverd
due to it is out of edk2 RISC-V contribution scope.

## EDK2 Build Target
"RISCV64" ARCH is the RISC-V architecture which currently supported and verified.
The verified RISC-V toolchain is https://github.com/riscv/riscv-gnu-toolchain
and the toolchain tag is "GCC5" which is declared in tools_def.txt.
Below is the edk2 build options for building RISC-V RV64 platform,
```
build -a RISCV64 -p Platform/{Vendor}/{Platform}/{Platform}.dsc -t GCC5
```
For example,
```
build -a RISCV64 -p Platform/SiFive/U5SeriesPkg/FreedomU500VC707Board/U500.dsc
-t GCC5
```

Make sure RISC-V toolchain is built succesfully and the toolchain binaries are
generated in somewhere you specified when building toolchain.
'GCC5_RISCV64_PREFIX' is the cross compilation prefix to toolchain binraries.
For example, set 'GCC5_RISCV64_PREFIX' to '~/RiscVToolchain/riscv64-unknown-elf-'
before you build RISC-V edk2 port.
