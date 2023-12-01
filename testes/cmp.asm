; $Id: cmp.asm

[compat nexfuse] ; all these new features come from the NexFUSE compiler

f:
; print 'FALSE'
  lsl R11,'F','A','L','S','E',0x0a
  each R11

t:
; print 'TRUE'
  lsl R10,'T','R','U','E',0x0a
  each R10

m:
; compare R1 and R2
  mov R1,0x0a
  mov R2,0x0a
; compare takes 4 arguments, two registers, a true subroutine, and a false
; subroutine
; in this case, we compare R1 and R2 and if they are equal, we jump to the `t'
; label, other wise we jump to the `f' label
  cmp R1,R2,t,f
