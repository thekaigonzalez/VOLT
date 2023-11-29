; $Id: put.asm

main:
; push 0x41 to reg 1 at 1
  push R1,0x41,1
; push 0x0a to reg 1 at 2
  push R1,0x0a,2

; pops from reg 1 to reg 2
  pop R1,1,R2
  pop R1,2,R2
  
; print out each byte in reg 2
  each R2

