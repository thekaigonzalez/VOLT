; $Id: syntax-overview.asm

; comment
# this is also a comment!

; subroutines in LR Assembly start with '@' traditionally, however, the flexible
; syntax that the VOLT compiler provides also allows you to declare subroutine
; names without the '@' prefix
s:
  echo 'A'

; The MAIN function is the function that does not contain any subroutine
; declaration
; on OpenLUD, this is the only subroutine supported, as it does not support true
; subroutines
m:
; `jmp' is the GOSUB instruction on NexFUSE, OpenLUD does not support this
; and that is why it's so important to ensure your code is maintainable
; and selects a certain compiler in order to maintain portability
  jmp s
; character literals are supported
  echo 'B'
; comments in subroutines are supported, but inline comments are not
; comments in subroutines must also be without indentation, as these comments
; are.

; that is purely style, and does not affect the semantics of the code

; hexadecimal numbers are supported
  echo 0x0a

; this code prints 'AB\n'
; no halt instruction is explicity required, but there in case early-exit is
; needed
  hlt
; every `hlt` instruction looks like a double-end in the bytecode formats, which
; you can view using LunarRED's `bytedump' disassembler
