#include "VGen.h"
#include "VDefaults.h"
#include <stdio.h>
#include <string.h>

struct VByteCode
{
  VObject *object;
  byte *code;
  int size;
  int capacity;
  int ptr;
};

byte
v_findStandardFunction (char *id, VCodeGen_Node *_functions)
{
  int i;

  for (i = 0; _functions[i].id; i++)
    {
      if (strcmp (_functions[i].id, id) == 0)
        {
          return _functions[i].data;
        }
    }

  return -127;
}

VCompilerSupport
v_findStandardCompiler (char *id, VCodeGen_Node *_functions)
{
  int i;

  for (i = 0; _functions[i].id; i++)
    {
      if (strcmp (_functions[i].id, id) == 0)
        {
          return _functions[i].compiler;
        }
    }

  return -127;
}

VByteCode *
v_newByteCode (VObject *object)
{
  if (object)
    {
      VByteCode *bc
          = v_allocateMemory (v_objectPool (object), sizeof (VByteCode));
      if (bc)
        {
          bc->object = object;
          bc->code = v_allocateMemory (v_objectPool (object),
                                       VOLT_DEFAULT_INCREMENT);
          if (bc->code != NULL)
            {
              bc->size = 0;
              bc->capacity = VOLT_DEFAULT_INCREMENT;
              return bc;
            }
        }
    }
  return NULL;
}

void
v_appendByteCode (VByteCode *code, byte data)
{
  if (!code)
    {
      return;
    }

  if (code->size == code->capacity)
    {
      code->capacity += VOLT_DEFAULT_INCREMENT;
      code->code = v_reallocateMemory (v_objectPool (code->object), code->code,
                                       code->capacity);
    }

  code->code[code->size] = data;
  code->size++;
}

int
v_byteCodeLength (VByteCode *code)
{
  if (!code)
    {
      return 0;
    }
  return code->size;
}

void
v_byteCodePrint (VByteCode *code)
{
  if (code == NULL)
    {
      printf ("warn: code is null\n");
      return;
    }
  for (int i = 0; i < code->size; i++)
    {
      printf ("%d ", code->code[i]);
    }

  printf ("\n");
}

VByteCode *
v_generateByteCode (VObject *object, VList *tokens, VCodeGen_Node *_functions,
                    VCompilerSupport compiler)
{
  if (!tokens)
    {
      return NULL;
    }

  VByteCode *main = v_newByteCode ((object));

  int pc = 0;

  for (int i = 0; i < v_listSize (tokens); i++)
    {
      VToken *token = v_listGetToken (tokens, i);

      if (token)
        {
          if (v_tokenType (token) == v_token_subr_header && pc == 0)
            {
            subroutine:
              // begin warnings
              if (compiler != v_compiler_std || compiler != v_compiler_any)
                {
                  if (compiler == v_compiler_openlud)
                    {
                      printf (
                          "note(%d): OpenLUD does not support subroutines, "
                          "skipping.\n",
                          i);
                      continue;
                    }

                  if (strlen (v_tokenName (token)) > 1
                      && compiler == v_compiler_nexfuse)
                    {
                      printf (
                          "warn: NexFUSE supports 1-character subroutines "
                          "only. Using first character of `%s' as subroutine "
                          "name\n",
                          v_tokenName (token));
                    }
                }
              // end warnings

              // VValue *value = v_tokenToValue (token);

              v_appendByteCode (main, 10);
              v_appendByteCode (main, v_valueToByte (token));

              pc++;
            }

          else if (v_tokenType (token) == v_token_subr_header && pc == 1)
            {
              v_appendByteCode (main, 0);  // END
              v_appendByteCode (main, 11); // ENDSUB
            }

          else if (v_tokenType (token) == v_token_ident && pc == 1)
            {
              byte fn
                  = v_findStandardFunction (v_tokenName (token), _functions);

              if (fn == -127)
                {
                  printf ("error: function is not found, `%s'\n",
                          v_tokenName (token));
                }

              v_appendByteCode (main, fn);
            }
          else
            {
              if (pc == 1)
                {
                  v_appendByteCode (main, v_valueToByte (token));
                }
            }
        }
    }

  return main;
}
