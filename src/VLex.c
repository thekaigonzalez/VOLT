#include "VLex.h"
#include "VBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum VLexerState
{
  v_lexer_start,      /* start state */
  v_lexer_subroutine, /* inside of a subroutine*/
  v_lexer_ignorant,   /* like a string (strings not allowed in LR Assembly, but
                         my point still stands)*/
  v_lexer_simple_literal,  /* simple literals ('a')*/
  v_lexer_param,           /* parameters (mov r1,0x42)*/
  v_lexer_legacy_header,   /* lunarred-style headers '@' */
  v_lexer_directive,       /*directives (preprocessing) ([directive <param1>
                              <param2>])*/
  v_lexer_directive_param, /*directive parameters*/
  v_lexer_inline_binary,   /*inline binary*/
  v_lexer_inline_rest,     /*the rest of inline*/
};

VList *
v_lex (VObject *object, char *str)
{
  int len = strlen (str);

  VLexerState state = v_lexer_start;
  VLexerState previous_state = v_lexer_start;

  VBuffer *buffer = v_newBuffer (object);
  VLexerState top_level_state = v_lexer_start;
  VLexerState string_past_state = v_lexer_start;

  if (!buffer)
    {
      return NULL;
    }

  VList *list = v_newList (object);

  for (int i = 0; i < len; i++)
    {
      char c = str[i];

      if (c == VTOKEN_SUB_DEF
          && (state == v_lexer_start || state == v_lexer_legacy_header
              || state == v_lexer_subroutine
              || state == v_lexer_param)) /* header: */
        {
          if (v_bufferLength (buffer) <= 0)
            {
              continue;
            }

          v_listAddToken (list,
                          v_newTokenPreset (object, v_copyBuffer (buffer),
                                            v_token_subr_header));

          v_clearBuffer (buffer);

          state = v_lexer_subroutine;
        }

      else if (c == VTOKEN_INLINE_BEGIN
               && (state != v_lexer_ignorant
                   || state != v_lexer_simple_literal))
        { /* {<code>}*/
          previous_state = state;
          state = v_lexer_inline_binary;
          v_clearBuffer (buffer);
        }

      else if (c == VTOKEN_DIRECTIVE_BEGIN
               && state == v_lexer_start) /* header: */
        {
          state = v_lexer_directive;
        }

      else if (c == VTOKEN_DIRECTIVE_END
               && (state == v_lexer_directive
                   || state == v_lexer_directive_param)
               && v_bufferLength (buffer)
                      > 0) /* [directive <param1> <param2>] */
        {
          if (state == v_lexer_directive)
            {
              v_listAddToken (list,
                              v_newTokenPreset (object, v_copyBuffer (buffer),
                                                v_token_directive));
            }

          else if (state == v_lexer_directive_param)
            {
              v_listAddToken (list,
                              v_newTokenPreset (object, v_copyBuffer (buffer),
                                                v_token_directive_param));
            }

          v_clearBuffer (buffer);
          state = v_lexer_start;
        }

      else if (VTOKEN_PARAM_DEF (c)
               && state
                      == v_lexer_directive) /* [directive <param1> <param2>] */
        {
          v_listAddToken (list,
                          v_newTokenPreset (object, v_copyBuffer (buffer),
                                            v_token_directive));

          state = v_lexer_directive_param;

          v_clearBuffer (buffer);
        }

      else if (VTOKEN_PARAM_DEF (c)
               && state == v_lexer_directive_param) /* [directive <param1>
                                                       <param2>] */
        {
          v_listAddToken (list,
                          v_newTokenPreset (object, v_copyBuffer (buffer),
                                            v_token_directive_param));
          v_clearBuffer (buffer);
        }

      else if (c == '\n' && state == v_lexer_ignorant)
        {
          state = top_level_state;
        }

      else if ((VTOKEN_PARAM_DEF (c) || (c == ';' || c == '#'))
               && state == v_lexer_subroutine
               && v_bufferLength (buffer) > 0) /* spacer between function name
                                                  and parameters */
        {
          if (v_bufferLength (buffer) <= 0)
            {
              continue;
            }
          v_listAddToken (
              list,
              v_newTokenPreset (object, v_copyBuffer (buffer), v_token_ident));

          top_level_state = state;

          state = v_lexer_param;

          v_clearBuffer (buffer);

          if (c == ';' || c == '#')
            {
              state = v_lexer_ignorant;
            }
        }

      else if (c == VTOKEN_SUBROUTINE_DEF_LEGACY
               && state == v_lexer_start) /* @header: */
        {
          state = v_lexer_legacy_header;
        }

      else if ((i == len - 1 || c == '\n')
               && state == v_lexer_subroutine) /* functions without
                                                        parameters */
        {
          if (v_bufferLength (buffer) <= 0)
            {
              continue;
            }
          if (!isspace (c))
            {
              v_appendBuffer (buffer, c);
            }

          v_listAddToken (
              list,
              v_newTokenPreset (object, v_copyBuffer (buffer), v_token_ident));

          v_clearBuffer (buffer);
        }

      else if (c == '#'
               || c == ';' && state != v_lexer_simple_literal
                      && state != v_lexer_ignorant) /* comments */
        {
          top_level_state = state;
          state = v_lexer_ignorant;
        }

      else if (c == '\'' && state != v_lexer_simple_literal
               && state != v_lexer_ignorant)
        {
          v_appendBuffer (buffer, c);
          string_past_state = state;
          state = v_lexer_simple_literal;
        }

      else if (c == ' ' && state == v_lexer_param
               && v_bufferLength (buffer) > 0)
        {
          printf ("volt: error: unexpected parameter separator\n");
          printf ("[C]: from (%s():%d)\n", __func__, __LINE__);
          printf ("volt: near: `%s'\n", v_copyBuffer (buffer));
          exit (1);
        }

      else if (c == '\'' && state == v_lexer_simple_literal
               && state != v_lexer_ignorant)
        {
          state = string_past_state;
          v_appendBuffer (buffer, c);
        }

      else if ((c == VTOKEN_PARAM_SEPARATOR || i == len - 1 || c == '\n')
               && state == v_lexer_param) /* R1,0x42 */
        {
          if (v_bufferLength (buffer) <= 0)
            {
              continue;
            }
          if (!isspace (c) && i == len - 1)
            {
              v_appendBuffer (buffer, c);
            }

          v_listAddToken (
              list,
              v_newTokenPreset (object, v_copyBuffer (buffer), v_token_param));

          v_clearBuffer (buffer);

          if (c == '\n')
            {
              state = top_level_state;
            }
        }
      else
        {
          if (!isspace (c) && state != v_lexer_ignorant
              && state != v_lexer_simple_literal && (!ispunct (c) && c != '-'))
            v_appendBuffer (buffer,
                            c); /* append the charaacter to the buffer*/

          if (state == v_lexer_simple_literal)
            {
              v_appendBuffer (buffer, c);
            }
        }
    }

  return list;
}
