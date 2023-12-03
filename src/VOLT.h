// $Id: VOLT.h

#ifndef VOLT_H
#define VOLT_H

#include "VBuffer.h"
#include "VObject.h"

/*defines a singular byte, this is used for codegen, NOTE: this does not mean
codegen can support higher than 8-bit data, however, this may be supported in
the future, when the 32-bit branch is merged into the main branch.*/
#define byte char

#define VOLT_VERSION "1.4.0"
#define VOLT_VERSION_LONG 120L

/* Number versions of the compilers which VOLT supports */
#define VOLT_COMPILER_OPENLUD 1
#define VOLT_COMPILER_NEXFUSE 2
#define VOLT_COMPILER_MERCURY 3

#endif // VOLT_H
