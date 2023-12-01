; $Id: inp.asm

[compat nexfuse] ; 'in' is not supported in OpenLUD

; If user types 'Hi'
h:
; create a new string in register 20
  lsl R20,'H','e','l','l','o','!',0x0a
  each R20

; If user does not type 'Hi'
b:
; create a new string in register 21
  lsl R21,'B','y','e','!','!',0x0a
  ; print it
  each R21

s:
; construct a welcome string
  lsl R6,'T','y','p','e',' ','"','H','i','"','!',0x0a
; print it
  each R6

m:
  jmp s
; get input
  in R1
  in R1

; create a separate string to test if 
; the input is what we wanted
  lsl R2,'H','i'
; if it is, goto `h', otherwise,
; assume they said bye
  cmp R1,R2,h,b
