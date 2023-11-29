; mov test

main:
; construct string on register 1
  mov R1, 'H'
  mov R1, 'e'
  mov R1, 'l'
  mov R1, 'l'
  mov R1, 'o'
; Print a new line
  each r1
  echo 0x0a
