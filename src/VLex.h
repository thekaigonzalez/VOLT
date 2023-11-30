// $Id: VLex.h

#ifndef VLEX_H
#define VLEX_H

/* lexer */

#include "VList.h"
#include "VObject.h"
#include "VToken.h"
#include "VValue.h"

#include <ctype.h>

#define VTOKEN_SUB_DEF ':'               // header:
#define VTOKEN_SUBROUTINE_DEF_LEGACY '@' // @header:
#define VTOKEN_PARAM_SEPARATOR ','       // ,
#define VTOKEN_PARAM_DEF(x) isspace (x)  // parameter
#define VTOKEN_DIRECTIVE_BEGIN '['       // [directive <param>]
#define VTOKEN_DIRECTIVE_END ']'         // ]

typedef enum VLexerState VLexerState;

VList *v_lex (VObject *object, char *str);

#endif // VLEX_H
