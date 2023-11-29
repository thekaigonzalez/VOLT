#include "VValue.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

struct VValue
{
  VMemoryPool *blk; /* parent */
  VValueType type;  /* type of value */
  union
  {
    char literal;
    int number;
    int nil;
  };
};

VValue *
v_newValue (VObject *object)
{
  VMemoryPool *pool = v_objectPool (object);

  VValue *v = v_allocateMemory (pool, sizeof (VValue));

  if (v)
    {
      v->blk = pool;

      v->type = v_nil;
      v->nil = 1;

      return v;
    }

  return NULL;
}

VValue *
v_newValueFromString (VObject *object, char *str)
{
  VMemoryPool *pool = v_objectPool (object);

  VValue *v = v_allocateMemory (pool, sizeof (VValue));

  // LR Assembly supports 2 types - char literals & numbers
  // numbers can be normal numbers, hexadecimal numbers, stuff like that
  // the reference assembler did NOT however support character literals,
  // this implementation of LR assembly does, as well as supports a compiler
  // extension, "nil"
  if (v)
    {
      if (strlen (str) == 0) /* empty string */
        {
          v->type = v_nil;
          v->nil = 1;
        }
      else if (str[0] == 'R' || str[0] == 'r') /* registers */
        {
          v->type = v_number;
          v->number = strtoull (str + 1, NULL, 10);
        }
      else if (str[0] == '0' && (str[1] == 'x')) /* obviously for base 16*/
        {
          v->type = v_number;
          v->number = strtoull (str, NULL, 16);
        }
      else if (isalpha (str[0]))
        {
          v->number = str[0];
          v->type = v_char_literal;
        }
      else if (isdigit (str[0])) /* if the first number is a digit we're gonna
                                    automatically assume that it's a number, if
                                    not then that's just cruel homie*/
        {
          v->type = v_number;
          v->number = strtoull (str, NULL, 10); /* base 10 number*/
        }

      else if (strcmp (str, "nil") == 0)
        {
          v->type = v_nil;
          v->nil = 1;
        }
      else
        {
          v->type = v_error_type;
          v->nil = 1;
        }

      return v;
    }
  else
    {
      printf ("warn: could not allocate memory for value\n");
      printf ("[C]: from (%s():%d)\n", __func__, __LINE__);
    }

  return NULL;
}

const char *
v_valueTypeString (VValueType type)
{
  switch (type)
    {
    case v_nil:
      return "<nil>";
    case v_char_literal:
      return "<literal>";
    case v_number:
      return "<number>";
    case v_error_type:
      return "<error>";
    default:
      return "<unknown>";
    }
  return NULL;
}

int
v_valueIsCharLiteral (VValue *value)
{
  if (!value)
    {
      return 0;
    }

  if (value->type == v_char_literal)
    {
      return 1;
    }

  return 0;
}

int
v_valueIsNumber (VValue *value)
{
  if (!value)
    {
      return 0;
    }

  if (value->type == v_number)
    {
      return 1;
    }

  return 0;
}

VValueType
v_valueType (VValue *value)
{
  if (!value)
    {
      return v_nil;
    }
  return value->type;
}

int
v_valueIsNil (VValue *value)
{
  if (!value)
    {
      return 1;
    }
  return value->nil == 1;
}

int
v_valueNumber (VValue *value)
{
  if (!value)
    {
      return -1;
    }

  if (value->type != v_number)
    {
      printf ("warn: expected 'number' got '%s'\n",
              v_valueTypeString (value->type));
      return -1;
    }

  return value->number;
}

char
v_valueCharLiteral (VValue *value)
{
  if (!value)
    {
      return '\0';
    }

  return value->literal;
}
