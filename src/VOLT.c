// $Id: VOLT.c

#include "VOLT.h"
#include "VGen.h"
#include "VLex.h"
#include "VObject.h"
#include "VToken.h"

#include "argw.h"

#include <stdio.h>
#include <string.h>

VCodeGen_Node standard[]
    = { { "null", 00, v_compiler_std },    { "echo", 40, v_compiler_std },
        { "hlt", 22, v_compiler_std },     { "int", 100, v_compiler_std },
        { "ret", 11, v_compiler_nexfuse }, { "jmp", 15, v_compiler_nexfuse },
        { "use", 70, v_compiler_nexfuse }, { "mov", 41, v_compiler_std },
        { "each", 42, v_compiler_std },    { "reset", 43, v_compiler_std },
        { "clear", 44, v_compiler_std },   { "push", 45, v_compiler_std },
        { "pop", 46, v_compiler_std },     { NULL, 0, 0 } };

int
main (int argc, char *argv[])
{
  VObject *root = v_newObject ();

  argw_init ();

  argw_flag ('o', "output", "Output directory", WString);
  argw_flag ('W', "compiler", "The compiler to use with VOLT", WString);

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

  char *filname = argw_positional (0);
  char *compiler = argw_str ('W');
  char *output_f = argw_str ('o');

  VCompilerSupport chosenCompiler = v_compiler_any;

  if (strcmp (compiler, "openlud") == 0)
    {
      chosenCompiler = v_compiler_openlud;
    }

  if (strcmp (compiler, "nexfuse") == 0)
    {
      chosenCompiler = v_compiler_nexfuse;
    }

  if (strcmp (compiler, "std") == 0)
    {
      chosenCompiler = v_compiler_std;
    }

  if (strcmp (compiler, "any") == 0)
    {
      chosenCompiler = v_compiler_any;
    }

  if (filname == NULL)
    {
      goto help_wanted;
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

  fwrite (byteCode->code, 1, byteCode->size, output);
  fclose (output);

  v_deleteObject (root);

  argw_exit (0);
}
