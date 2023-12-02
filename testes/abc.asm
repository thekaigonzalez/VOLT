; $Id: abc.asm

; R1 - output
; R2 - input
; R3 - output

; R4 - comparison 1 (abc)
; R5 - comparison 2 (abd)
; R6 - comparison 3 (abe)

; asks user to print 'abc' and prints different 
; messages based on inputs

[compat mercury]

; ======= abc.asm ======

; let user know what to type
a:
; first half of welcome message
  lsl R1,'T','y','p','e',' '
  jmp b

b:
; second half of welcome message
  lsl R1,'a','b','c','!',0x0a
; print it (first half + second half)
  each R1
; jump to input prompt
  jmp i

; input
c:
; R2 is our input buffer
; request input
  in R2
; compare R2 to R4 (R4 = abc)
; R4 is what we want
  cmp R2,R4,d,n

n:
  cmp R2,R5,d,v

v:
  cmp R2,R6,d,c

l:
  lsl R8,'W','r','o','n','g','!',0x0a
  each R8

d:
  lsl R10,'D','O','N','E',0x0a
  each R10

u:
  lsl R9,'D','i','d',' ','y','o','u',' ','w','o','r','k','?',0x0a
  each R9

i:
; add a nice prompt
  lsl R3,'>',' '

; print it
  each R3

; jump to the input
  jmp c

m:
  lsl R4,'a','b','c'
  lsl R5,'a','b','e'
  lsl R6,'a','b','d'

  jmp a
