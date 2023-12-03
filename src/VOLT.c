// $Id: VOLT.c

#include "VOLT.h"
#include "VGen.h"
#include "VLex.h"
#include "VObject.h"
#include "VToken.h"

#include "argw.h"

#include <stdio.h>
#include <string.h>

VCodeGen_Node standard[] = { { "null", 00, v_compiler_std },
                             { "echo", 40, v_compiler_std },
                             { "hlt", 22, v_compiler_nexfuse },
                             { "init", 100, v_compiler_std },
                             { "ret", 0x80, v_compiler_nexfuse },
                             { "jmp", 15, v_compiler_nexfuse },
                             { "use", 70, v_compiler_nexfuse },
                             { "mov", 41, v_compiler_std },
                             { "each", 42, v_compiler_std },
                             { "reset", 43, v_compiler_std },
                             { "clear", 44, v_compiler_std },
                             { "push", 45, v_compiler_std },
                             { "pop", 46, v_compiler_std },
                             { "add", 47, v_compiler_nexfuse },
                             { "lar", 48, v_compiler_nexfuse },
                             { "lsl", 49, v_compiler_nexfuse },
                             { "in", 50, v_compiler_nexfuse },
                             { "cmp", 51, v_compiler_nexfuse },
                             { "inc", 52, v_compiler_nexfuse },
                             { "rep", 53, v_compiler_nexfuse },
                             { "rcl", 54, v_compiler_nexfuse },
                             { "int", 55, v_compiler_mercury },
                             { NULL, 0, 0 } };

int
main (int argc, char *argv[])
{
  VObject *root = v_newObject ();

  argw_init ();

  argw_flag ('o', "output", "Output directory", WString);
  argw_flag ('W', "compiler", "The compiler to use with VOLT", WString);
  wFlag *warnings = argw_flag (
      'U', "warning", "Specify certain warnings (new to v1.4.0)", WList);

  argw_default ('o', "a.bin");
  argw_default ('c', "any");

  argw_parse (argc, argv);

  if (wArgParserHelpWanted (parser))
    {
    help_wanted:
      printf ("VOLT (Versatile-Optimized-Language-Transpiler)\n");
      printf ("Version: %s\n", VOLT_VERSION);
      wArgParserPrintHelp (parser, argv[0], "[-oc]");

      return 0;
    }

  for (int i = 0; i < wValueListSize (wFlagValue (warnings)); i++)
    {
      if (strcmp (wValueListAt (wFlagValue (warnings), i), "no-big-registers")
          == 0)
        {
          VMemoryPool *pool = v_objectPool (root);
          v_W_setWarnForBigRegisters (pool, false);
        }
    }

  char *filname = argw_positional (0);
  char *compiler = argw_str ('W');
  char *output_f = argw_str ('o');

  VCompilerSupport chosenCompiler = v_compiler_any;

  if (filname == NULL)
    {
      goto help_wanted;
    }

  if (compiler == NULL)
    {
      printf ("warn: no compiler specified, using 'any'\n");
      compiler = "any";
    }
  if (strcmp (compiler, "openlud") == 0)
    {
      chosenCompiler = v_compiler_openlud;
    }

  else if (strcmp (compiler, "nexfuse") == 0)
    {
      chosenCompiler = v_compiler_nexfuse;
    }

  else if (strcmp (compiler, "std") == 0)
    {
      chosenCompiler = v_compiler_std;
    }

  else if (strcmp (compiler, "any") == 0)
    {
      chosenCompiler = v_compiler_any;
    }

  else if (strcmp (compiler, "mercury") == 0)
    {
      chosenCompiler = v_compiler_mercury;
    }

  else
    {
      printf ("error: unknown compiler `%s'\n", compiler);

      argw_exit (1);
    }

  FILE *fp = fopen (filname, "r");

  if (!fp)
    {
      printf ("volt: could not open `%s'\n", filname);

      argw_exit (1);
    }

  VBuffer *buffer = v_newBuffer (root);

  if (!buffer)
    {
      argw_exit (1);
    }

  char c;
  while ((c = fgetc (fp)) != EOF)
    {
      v_appendBuffer (buffer, c);
    }

  fclose (fp);

  VList *lexed = v_lex (root, v_copyBuffer (buffer));

  VByteCode *byteCode
      = v_generateByteCode (root, lexed, standard, chosenCompiler);

  FILE *output = fopen (output_f, "wb");

  if (!output)
    {
      argw_exit (1);
    }

  fwrite (byteCode->code, sizeof (byte), byteCode->size * sizeof (byte),
          output);

  fclose (output);

  v_deleteObject (root);

  argw_exit (0);
}
