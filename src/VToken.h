// $Id: VToken.h

#ifndef VTOKEN_H
#define VTOKEN_H

#include "VOLT.h"
#include "VObject.h"
#include "VValue.h"

typedef struct VToken VToken;
typedef enum
{
  v_token_ident,       // identifier
  v_token_param,       // parameter
  v_token_subr_header, // subroutine header
  v_token_none,        // none
} VTokenType;

/* allocate a new token on object OBJECT */
VToken *v_newToken (VObject *object, char *value);

/*simple function to set value and type in one*/
VToken *v_newTokenPreset (VObject *object, char *value, VTokenType type);

/*sets the value of the token*/
void v_setTokenValue (VToken *token, char *n);

/*sets the type of the token*/
void v_setTokenType (VToken *token, VTokenType type);

/* prints a token */
void v_tokenPrint (VToken *token);

/*verify if a token is a proper identifier*/
int v_tokenIsIdentifier (VToken *token);

/*convert a token to a value*/
VValue *v_tokenToValue (VToken *token);

/*gets the name of the token*/
char *v_tokenName (VToken *token);

/*gets the type of the token*/
VTokenType v_tokenType (VToken *token);

/*converts a value to a byte*/
byte v_valueToByte (VToken *token);

#endif // VTOKEN_H
