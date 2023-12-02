; $Id: 32-bit.asm

; run this code with Mercury's 32 bit mode
; otherwise, expect some weirdness

[compat mercury]

28723:
  echo 'A'

m:
  jmp 28723
