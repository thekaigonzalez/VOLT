<!-- $Id: index.md $ -->
# Mercury VOLT

VOLT is an extremely small, fast, and portable implementation of LR Assembly. It
is designed to compile for the NexFUSE & OpenLUD platforms, as an alternative to
the standard LunarRED compiler.

```asm
; mov test

main:
; construct string on register 1
  mov R1, 'H'
  mov R1, 'e'
  mov R1, 'l'
  mov R1, 'l'
  mov R1, 'o'
; Print a new line
  each r1
  echo 0x0a
```

## VOLT Compiler Support

VOLT supports the following formats (byte 0 shift):

* NexFUSE
* OpenLUD
