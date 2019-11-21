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
RiscVPlatformPkg  - RISC-V platform package. This package provides RISC-V
                    platform common modules, libraries, PCDs and definitoins.
```
## Toolchain of RISC-V EDK2 port
https://github.com/riscv/riscv-gnu-toolchain
You have to clone the toolchain from above link for building RISC-V edk2 port.

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
