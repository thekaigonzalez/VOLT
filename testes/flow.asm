; $Id: flow.asm

; control flow test
; equivalent to:
; if (R1 == R2) {
;   print("FIRST");
; }
; else if (R1 == R3) {
;   print("SECOND");
; }
; else 
; {
;   print("THIRD");
; }

[compat mercury]

1:
; if (R1 == R2)
  cmp R1,R2,11,2

11:
; print("FIRST")
  lsl R10,'F','I','R','S','T',0x0a
  each R10

2:
; else if (R1 == R3)
  cmp R1,R3,12,3

12:
; print("SECOND")
  lsl R10,'S','E','C','O','N','D',0x0a
  each R10

3:
; else
; print("THIRD")
  lsl R10,'T','H','I','R','D',0x0a
  each R10

m:
; try changing these values to see how the flow changes
  lsl R1,'A','B','C',0x0a
  lsl R2,'A','B','C',0x0a
  lsl R3,'A','C','C',0x0a

; jump to comparisons
  jmp 1
