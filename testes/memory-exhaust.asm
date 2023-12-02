; $Id: memory-exhaust.asm
; test this file with NexFUSE & Mercury to see exhaustion differences

[compat mercury]

a:
  cmp R1,R2,c,b

b:
  inc R1
  jmp a

c:
  lsl R10,'D','O','N','E',0x0a
  each R10

m:
  mov R2,1000000
  jmp a
