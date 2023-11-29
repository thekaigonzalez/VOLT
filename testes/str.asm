; $Id: str.asm

@1:
  mov R1, 'H'
  mov R1, 'e'
  mov R1, 'l'
  mov R1, 'l'
  mov R1, 'o'
  mov R1, 0x0a

@2:
  mov R1, 'W'
  mov R1, 'o'
  mov R1, 'r'
  mov R1, 'l'
  mov R1, 'd'
  mov R1, 0x0a

m:
  jmp 1
  jmp 2

  each R1
  each R2
