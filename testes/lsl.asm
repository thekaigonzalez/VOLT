; $Id: lsl.asm

[compat nexfuse] ; `lsl' is not supported in OpenLUD

m:
; lsl can consecutively add bytes to a register
  lsl R1,'H','e','l','l','o',0x0a
  each R1
  
; lsl can add bytes to a register
  lsl R2,'W','o','r','l','d',0x0a
  each R2

; `in' can get input from the keyboard
; depending on which VM you are using, this function may vary,
; in NexFUSE, `in' is a binding to fgetc(stdin)
  in R12
  each R12
