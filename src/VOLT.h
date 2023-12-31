// $Id: VOLT.h

#ifndef VOLT_H
#define VOLT_H

#include "VBuffer.h"
#include "VObject.h"

#ifdef BIT32
#define byte __uint32_t
#else
#define byte unsigned char
#endif

#ifdef BIT32
#define VOLT_VERSION "1.5.0 (32-bit)"
#else
#define VOLT_VERSION "1.5.0 (8-bit)"
#endif

#define VOLT_VERSION_LONG 140L

/* Number versions of the compilers which VOLT supports */
#define VOLT_COMPILER_OPENLUD 1
#define VOLT_COMPILER_NEXFUSE 2
#define VOLT_COMPILER_MERCURY 3

#endif // VOLT_H
