; $Id: str.asm

[compat nexfuse] ; nexfuse supports `jmp' and `hlt'

@1:
  mov R1,'h'
  mov R1,'e'
  mov R1,'l'
  mov R1,'l'
  mov R1,'o'
  mov R1,' '

@2:
  mov R1,'w'
  mov R1,'o'
  mov R1,'r'
  mov R1,'l'
  mov R1,'d'
  mov R1,'!'
  mov R1, 0x0a

m:
  jmp 1
  jmp 2

  each R1
  each R2
