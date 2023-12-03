; $Id: large.asm

1:
  mov R1,1000000000
  rcl R1,0

m:
  jmp 1
