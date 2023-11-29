// $Id: VGen.h

#ifndef VGEN_H
#define VGEN_H

#include "VList.h"
#include "VOLT.h"
#include "VObject.h"

typedef enum VCompilerSupport
{
  v_compiler_openlud, /* OpenLUD compatibility*/
  v_compiler_any, /* note: not recommended if you are compiling for a certain
                     platform */
  v_compiler_nexfuse, /* NexFUSE-based compatibility*/
  v_compiler_std
  = v_compiler_openlud | v_compiler_nexfuse, /* standard compliant functions*/
} VCompilerSupport;

typedef struct VCodeGen_Node
{
  char *id;
  byte data;
  VCompilerSupport compiler;
} VCodeGen_Node;

// VCodeGen_Node functions[]
//     = { { "null", 00, v_compiler_std },
//         { "echo", 40, v_compiler_std },
//         { "mov", 41, v_compiler_std },
//         { "each", 42, v_compiler_std },  // POPAll [register]
//         { "clr", 43, v_compiler_std },   // CLR [register]
//         { "reset", 44, v_compiler_std }, // RESET [...]
//         { "push", 45, v_compiler_std },  // PUSH [register] [byte]
//         [position] { "get", 46,
//           v_compiler_std }, // GET [register] [position] [output_register]
//         { "halt", 22, v_compiler_std },   // END
//         { "endsub", 11, v_compiler_std }, // EXIT
//         { "init", 100, v_compiler_std },  // INIT [register]};
//         { NULL } };

/* find a function in a list of VCodeGen_Nodes*/
byte v_findStandardFunction (char *id, VCodeGen_Node *_functions);

/* find a function's compiler support in a list of VCodeGen_Nodes*/
VCompilerSupport v_findStandardCompiler (char *id, VCodeGen_Node *_functions);

struct VByteCode
{
  VObject *object;
  byte *code;
  int size;
  int capacity;
  int ptr;
};

typedef struct VByteCode VByteCode;

VByteCode *v_newByteCode (VObject *object);
void v_appendByteCode (VByteCode *code, byte data);
int v_byteCodeLength (VByteCode *code);
void v_byteCodePrint (VByteCode *code);

VByteCode *v_generateByteCode (VObject *object, VList *tokens,
                               VCodeGen_Node *_functions,
                               VCompilerSupport compiler);

#endif // VGEN_H
