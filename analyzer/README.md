# VOLT Analyzer - Vulnerability Analyzer

Allows analysis of VOLT binary files.

Potential output:

```
VOLT Binary Analyzer v1.0.0
Copyright (C) Fake VOLT Foundation

basic information
  analyzer version  : v1.0.0
  file              : a.bin
  length            : {44} bytes
  architecture      : 32-bit
deep analysis:
  note: `1277' subroutine defined here
    problematic: subroutine label too high (segmentation fault)
    problematic: the label in question: `1277'

  note: `1277' subroutine called here
    problematic: gosub calls label too high, could potentially cause segmentation fault
    problematic: the label in question: `1277'
  my guess is this file is designed for `Mercury PIC (potentially 32-bit)'
```
