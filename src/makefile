# $Id: makefile
srcs=VMemoryPool.c VObject.c VValue.c VToken.c VList.c VLex.c VBuffer.c VGen.c

all:
	clang-format *.c --style=GNU -i
	clang-format *.h --style=GNU -i
	gcc VTests.c $(srcs) -o p_tests -g
