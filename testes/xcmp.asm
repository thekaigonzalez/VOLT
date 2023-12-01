; $Id: xcmp.asm

[compat nexfuse]

s:
  inc R1

m:
  rep s,65

; should print 'A' once
  each R1
