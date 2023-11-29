#include "VToken.h"
#include "VOLT.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

struct VToken
{
  VObject *blk;    /* parent */
  char *name;      /* name of the token */
  VTokenType type; /* type of token */
};

VToken *
v_newToken (VObject *object, char *value)
{
  VMemoryPool *pool = v_objectPool (object);

  VToken *token = v_allocateMemory (pool, sizeof (VToken));

  if (token)
    {
      token->blk = object;
      token->type = v_token_none;

      if (value)
        {
          token->name = value;
        }
      else
        {
          printf (
              "warn: attempted to create a token without a proper value\n");
          return NULL;
        }

      return token;
    }

  printf ("warn: could not find memory for token\n");

  return NULL;
}

VToken *
v_newTokenPreset (VObject *object, char *value, VTokenType type)
{
  VToken *token = v_newToken (object, value);

  if (token)
    {
      token->type = type;
      return token;
    }
  else
    {
      printf ("warn: could not allocate memory for token\n");
      return NULL;
    }
}

void
v_setTokenValue (VToken *token, char *n)
{
  if (!token)
    {
      printf ("warn: attempted to set a token value without a token\n");
      return;
    }

  if (!n)
    {
      printf ("warn: attempted to set a token value to NULL\n");
      return;
    }

  token->name = n;
}

void
v_setTokenType (VToken *token, VTokenType type)
{
  if (!token)
    {
      printf ("warn: attempted to set a token type without a token\n");

      return;
    }
  token->type = type;
}

void
v_tokenPrint (VToken *token)
{
  if (!token)
    {
      printf ("[C]: from (%s():%d)\n", __func__, __LINE__);
      return;
    }

  printf ("[%d]: %s\n", (token->type), token->name);
}

int
v_tokenIsIdentifier (VToken *token)
{
  if (!token)
    {
      return 0;
    }
  for (int i = 0; i < strlen (token->name); i++)
    {
      if (!isalpha (token->name[i]))
        {
          return 0;
        }
    }
  return 1;
}

VValue *
v_tokenToValue (VToken *token)
{
  if (!token)
    {
      return NULL;
    }

  return v_newValueFromString (token->blk, token->name);
}

char *
v_tokenName (VToken *token)
{
  if (!token)
    {
      return NULL;
    }
  return token->name;
}

VTokenType
v_tokenType (VToken *token)
{
  if (!token)
    {
      return v_token_none;
    }
  return token->type;
}

byte
v_valueToByte (VToken *token)
{
  if (!token)
    {
      return 0;
    }

  if (v_valueIsNil (v_tokenToValue (token)))
    {
      return -127;
    }

  if (v_valueIsCharLiteral (v_tokenToValue (token)))
    {
      return (byte)v_valueCharLiteral (v_tokenToValue (token));
    }

  if (v_valueIsNumber (v_tokenToValue (token)))
    {
      return (byte)v_valueNumber (v_tokenToValue (token));
    }

  return 0;
}
