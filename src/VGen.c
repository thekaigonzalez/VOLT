#include "VGen.h"
#include "VDefaults.h"
#include <stdio.h>
#include <string.h>

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
      printf ("%d,", code->code[i]);
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
  VByteCode *tmp = v_newByteCode ((object));

  int state = 0;
  int last_was_main = 0;

  for (int i = 0; i < v_listSize (tokens); i++)
    {
      VToken *token = v_listGetToken (tokens, i);

      if (!token)
        return NULL;

      if (v_tokenType (token) == v_token_subr_header
          && (state == 0 || state == 2))
        {
          if (state == 2)
            {
              for (int j = 0; j < v_byteCodeLength (tmp); j++)
                {
                  v_appendByteCode (main, tmp->code[j]);
                }

              v_appendByteCode (main, 0);

              memset (tmp->code, 0, tmp->capacity);
              tmp->size = 0;
            }
          // start warnings
          if (compiler != v_compiler_any
              || compiler != v_compiler_nexfuse
                     && v_tokenName (token)[0] != 'm')
            {
              if (compiler == v_compiler_std && compiler != v_compiler_nexfuse)
                {
                  printf (
                      "error: subroutines are not supported in LR Assembly "
                      "stanards (-Wstd)\n");
                  exit (1);
                }
              if (compiler == v_compiler_openlud)
                {
                  printf ("note: OpenLUD does not support subroutines, can "
                          "not compile (-Wopenlud)\n");
                  exit (1);
                }

              if (compiler == v_compiler_nexfuse
                  && strlen (v_tokenName (token)) > 1)
                {
                  printf ("warn: NexFUSE only supports one character "
                          "subroutines, will use the first letter of `%s` as "
                          "the subroutine name (-Wnexfuse)\n",
                          v_tokenName (token));
                }
            }
          if (state == 2 && !last_was_main)
            {
              v_appendByteCode (main, 22);
              v_appendByteCode (main, 11); // RET

              last_was_main = 0;
            }
          // end warnings
          state = 1;

          if (v_valueToByte (token) != 'm')
            {
              /*initialize the section*/
              v_appendByteCode (main, 10); // SUB
              v_appendByteCode (main, v_valueToByte (token));
            }
          else
            {
              last_was_main = 1;
            }
        }
      else if (v_tokenType (token) == v_token_ident && state == 1)
        {
        func:
          state = 2;

          byte f = v_findStandardFunction (v_tokenName (token), _functions);
          VCompilerSupport cs
              = v_findStandardCompiler (v_tokenName (token), _functions);

          if (compiler != v_compiler_any
              && (cs != v_compiler_std || cs != v_compiler_any))
            {
              if (cs == v_compiler_std)
                {
                  goto breakout;
                }
              if ((cs == v_compiler_openlud && compiler != v_compiler_openlud))
                {
                  printf ("error: function `%s` not supported by OpenLUD\n",
                          v_tokenName (token));
                  exit (1);
                }

              else if ((cs == v_compiler_nexfuse
                        && compiler != v_compiler_nexfuse))
                {
                  printf ("error: function `%s` not supported by NexFUSE\n",
                          v_tokenName (token));
                  exit (1);
                }
            }
        breakout:

          if (f != -127)
            {
              v_appendByteCode (main, f);
            }
          else
            {
              printf ("warn: unknown function `%s`\n", v_tokenName (token));
            }
        }

      else if (v_tokenType (token) == v_token_ident && state == 2)
        {
          for (int j = 0; j < v_byteCodeLength (tmp); j++)
            {
              v_appendByteCode (main, tmp->code[j]);
            }

          memset (tmp->code, 0, tmp->capacity);
          tmp->size = 0;

          v_appendByteCode (main, 0);

          goto func;
        }
      else
        {
          if (v_tokenType (token) == v_token_param && state == 2)
            {
              v_appendByteCode (tmp, v_valueToByte (token));
            }
        }
    }
  if (tmp->size > 0)
    {
      for (int j = 0; j < v_byteCodeLength (tmp); j++)
        {
          v_appendByteCode (main, (tmp->code[j]));
        }
      v_appendByteCode (main, 0);
    }

  if (compiler == v_compiler_openlud)
    {
      v_appendByteCode (main, 12); // END in OpenLUD
    }
  else
    {
      v_appendByteCode (main, 22); // Standard END
    }

  return main;
}
