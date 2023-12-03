# VOLT (Versatile-Optimized-Language-Transpiler)

VOLT is an extremely small, fast, and portable implementation of LR Assembly. It
is designed to compile for the NexFUSE & OpenLUD platforms, as an alternative to
the standard LunarRED compiler.

This is the 32-bit version of VOLT, it generates 32-bit binaries, with 4 byte long
integers. The `main` branch holds 8-bit code generation. This branch is for
MercuryPIC's 32-Bit Virtual Machine architecture.

## To install

```bash
git clone https://github.com/thekaigonzalez/VOLT.git
cd VOLT && cd src
make
```

## Options

```bash
volt -h
```

### -o, --output -> The output filename

Sets the output filename to the specified filename. The default filename is
`a.bin` if no filename is specified.

### -W, --compiler -> The compiler support for VOLT

Selects the compiler support for VOLT.

Do take note that this option is essentially the equivalent to warnings, hence
the short form `-W`.

```bash
volt -Wnexfuse # standard NexFUSE compiler features ONLY
volt -Wopenlud # standard OpenLUD compiler features ONLY

volt -Wmercury # standard Mercury compiler features ONLY, 
               # NOTE: this option MUST be used to compile 
               # binaries for the MercuryPIC VM, as it modifies 
               # statement delimiters to be supported by MercuryPIC

volt -Wstd # standard NexFUSE + OpenLUD compiler features ONLY, compiler/bytecode extensions disabled

# NOTE: this option disables all compiler warnings, 
# this is primarily meant for debugging/quick build/run cycles, 
# do not use if you are uncertain about your code standards.
volt -Wany # any compiler features allowed
```
