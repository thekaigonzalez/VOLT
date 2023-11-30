# always initialize registers that your program may use
# if you don't your program may have unexpected behavior
# one-letter subroutine names are allowed in NexFUSE but not OpenLUD
# in the future, there may be fewer restrictions and multiple-letter
# subroutines will be allowed
@m:
# move 0x42 into R1
  mov R1,0x42

# move newline into R1
  mov R1,0x0a 

# output each byte in R1 
  each R1
