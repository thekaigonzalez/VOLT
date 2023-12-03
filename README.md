# VOLT (Versatile-Optimized-Language-Transpiler)

VOLT is an extremely small, fast, and portable implementation of LR Assembly. It
is designed to compile for the NexFUSE & OpenLUD platforms, as an alternative to
the standard LunarRED compiler.

NOTE: to configure VOLT to generate 8-bit binaries, open VOLT.h and change

```c
#define byte int
```

to

```c
#define byte unsigned char
```

By default the makefile will already build an 8-bit and 32-bit executable,
allowing you to compile code for 8-bit VMs or 32-bit VMs.

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

### -S, --compiler -> The compiler support for VOLT

Selects the compiler support for VOLT.

```bash
volt -S nexfuse # standard NexFUSE compiler features ONLY
volt -S openlud # standard OpenLUD compiler features ONLY

volt -S mercury # standard Mercury compiler features ONLY, 
               # NOTE: this option MUST be used to compile 
               # binaries for the MercuryPIC VM, as it modifies 
               # statement delimiters to be supported by MercuryPIC

volt -S std # standard NexFUSE + OpenLUD compiler features ONLY, compiler/bytecode extensions disabled

# NOTE: this option disables all compiler warnings, 
# this is primarily meant for debugging/quick build/run cycles, 
# do not use if you are uncertain about your code standards.
volt -S any # any compiler features allowed
```

## Compatible Formats

* [NexFUSE](https://github.com/thekaigonzalez/NexFUSE)
* [OpenLUD OBI](https://github.com/thekaigonzalez/OpenLUD-OBI)
* [MercuryPIC](https://github.com/thekaigonzalez/MercuryPIC)
