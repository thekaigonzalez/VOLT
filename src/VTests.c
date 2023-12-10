// $Id: PTests.c

#include "VBuffer.h"
#include "VGen.h"
#include "VLex.h"
#include "VList.h"
#include "VMemoryPool.h"
#include "VObject.h"
#include "VToken.h"
#include "VValue.h"

#include <assert.h>
#include <stdio.h>

#define passed() printf ("test %s passed\n", __func__)

int
test_memory_pool (void)
{
  VMemoryPool *pool = v_newMemoryPool (1000, 500);

  char *str = v_allocateMemory (pool, 12);

  str[0] = 'h';
  str[1] = 'e';
  str[2] = 'l';
  str[3] = 'l';
  str[4] = 'o';
  str[5] = ' ';
  str[6] = 'w';
  str[7] = 'o';
  str[8] = 'r';
  str[9] = 'l';
  str[10] = 'd';
  str = v_reallocateMemory (pool, str, 20);

  str[11] = '?';
  str[12] = '!';
  str[13] = '\0';

  printf ("%s\n", str);

  v_deleteMemoryPool (pool);

  passed ();
  return 0;
}

int
test_object (void)
{
  VObject *o = v_newObject ();

  VMemoryPool *p = v_objectPool (o);

  v_deleteObject (o);

  passed ();
  return 0;
}

int
test_values (void)
{
  VObject *env = v_newObject ();

  VValue *v = v_newValueFromString (env, "0x100");

  printf ("type: %s\n", v_valueTypeString (v_valueType (v)));
  printf ("value: %d\n", v_valueNumber (v));

  v_deleteObject (env);
  passed ();
  return 0;
}

int
test_token (void)
{
  VObject *env = v_newObject ();

  VToken *t = v_newTokenPreset (env, "test", v_token_ident);

  assert (t);

  v_deleteObject (env);

  passed ();
  return 0;
}

int
test_list (void)
{
  VObject *env = v_newObject ();

  VList *l = v_newList (env);

  v_listAddToken (l, v_newTokenPreset (env, "test", v_token_ident));
  v_listAddToken (l, v_newTokenPreset (env, "0x01", v_token_param));

  v_deleteObject (env);

  passed ();
  return 0;
}

int
test_lex (void)
{
  VObject *env = v_newObject ();

  VBuffer *buf = v_newBuffer (env);

  FILE *f = fopen ("../testes/example.asm", "r");

  if (f)
    {
      char c;
      while ((c = fgetc (f)) != EOF)
        {
          v_appendBuffer (buf, c);
        }

      fclose (f);
    }

  VList *l = v_lex (env, v_copyBuffer (buf));

  v_listPrint (l);

  v_deleteObject (env);

  passed ();
  return 0;
}

int
test_rulesets (void)
{
  VObject *env = v_newObject ();

  VCodeGen_Node functions[]
      = { { "null", 13, v_compiler_std }, { NULL, 0, 0 } };

  printf ("%d\n", v_findStandardFunction ("null", functions));

  v_deleteObject (env);
  passed ();
  return 0;
}

int
test_bytecode (void)
{
  VObject *env = v_newObject ();

  VByteCode *bytecode = v_newByteCode (env);

  v_appendByteCode (bytecode, 10);
  v_appendByteCode (bytecode, 20);
  v_appendByteCode (bytecode, 30);

  v_error (env, "test error");

  v_byteCodePrint (bytecode);

  v_deleteObject (env);

  passed ();
  return 0;
}

int
test_codegen (void)
{
  VObject *env = v_newObject ();

  VBuffer *buf = v_newBuffer (env);

  VList *l = v_lex (env, "# hello\nz:\n\tmov R1,0x46\n\thlt");

  v_listPrint (l);

  VCodeGen_Node fns[] = { { "mov", 41, v_compiler_std },
                          { "hlt", 22, v_compiler_std },
                          { NULL, 0, 0 } };

  VByteCode *finished = v_generateByteCode (env, l, fns, v_compiler_nexfuse);

  v_byteCodePrint (finished);

  v_deleteObject (env);

  passed ();
  return 0;
}

int
main (void)
{
  test_memory_pool ();
  test_object ();
  test_values ();
  test_token ();
  test_list ();
  test_lex ();
  test_rulesets ();
  test_bytecode ();
  test_codegen ();
  return 0;
}
