Author: Christian Franklin
Date: 03/12/2025

Project Pluto

Pluto is a simple RISC-V disassembler specifically for the RISCV32I base instructions. 
It currently only supports disassembling 32-bit, statically linked ELF binaries. Pluto supports
symbol resolution for B instructions and J instrutions.

Todo:
- Add support for the various extensions (Atomic, Floating Point, etc)
- Add support for halfword size instructions (namely the C extension)
- Add alias support
