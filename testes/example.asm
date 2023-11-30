# Copyright 2019-2023 Kai D. Gonzalez
# more code here ...

; compat directive allows you to enforce which bytecode format is used
; this directive will error if the format is not switched
; this directive will also error if the format is not supported
[compat nexfuse]

@8:
  echo 0x47

@m:
  echo 'A'
  jmp 8
