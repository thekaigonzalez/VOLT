; $Id: normal.asm

; VOLT v1.5 supports inline instructions
; you can insert any raw data into VOLT code
; and it will be executed at runtime
; this is primarily for deep debugging/understanding 
; of instructions, having the user insert raw data that 
; would otherwise be inserted by codegen

s:
  echo 'A'
m:
  jmp s
