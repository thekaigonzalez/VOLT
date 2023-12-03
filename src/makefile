# $Id: makefile
srcs=VMemoryPool.c VObject.c VValue.c VToken.c VList.c VLex.c VBuffer.c VGen.c
warglib_sources=wArgParser.c wFlag.c wMem.c wName.c wOptions.c wStraggly.c wValue.c

all:
	clang-format *.c --style=GNU -i
	clang-format *.h --style=GNU -i
	gcc VTests.c $(srcs) -o p_tests -g
	gcc VOLT.c -o volt32 $(srcs) $(warglib_sources) -g -O2
