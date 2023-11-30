; $Id: syntax-overview.asm

; comment
# this is also a comment!

; there's also syntax enforcement via the NEW compiler directives, these are
; directives which are ignored at codegen, meaning that they are only visible by
; the compiler and will not be visible at runtime
[compat nexfuse]

; subroutines in LR Assembly start with '@' traditionally, however, the flexible
; syntax that the VOLT compiler provides also allows you to declare subroutine
; names without the '@' prefix
s:
  echo 'A'

; The MAIN function is the function that does not contain any subroutine
; declaration
; on OpenLUD, this is the only subroutine supported, as it does not support true
; subroutines
; do note that if you use 'main' or any longer words as subroutine names, they
; will be truncated into just 1 character, the starting character, the compiler
; will warn you once it detects this due to NexFUSE not supporting multi-letter
; subroutine headers.
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

; `nil' is also supported in VOLT, however, as this functionality is not
; supported in LR Assembly, it is not supported in OpenLUD either
; echo can print nil, and since nil is the separating factor in both formats,
; nil is equal to -127, or the max value of a byte, negatively

; this character looks crazy
  echo nil

; this code prints 'AB\n'
; no halt instruction is explicity required, but there in case early-exit is
; needed
; however, this functionality is only supported when compiling for NexFUSE
  ; hlt
; every `hlt` instruction looks like a double-end in the bytecode formats, which
; you can view using LunarRED's `bytedump' disassembler
