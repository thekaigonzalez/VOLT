; $Id: lsl.asm

m:
; lsl can consecutively add bytes to a register
  lsl R1,'H','e','l','l','o',0x0a
  each R1
  
; lsl can add bytes to a register
  lsl R2,'W','o','r','l','d',0x0a
  each R2
