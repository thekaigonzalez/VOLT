// $Id: VValue.h

#ifndef VVALUE_H
#define VVALUE_H

/*define values*/

#include "VObject.h"

/* contains information for a variant-style value in LR Assembly
not much work needs to really be done, since LR Assembly doesn't have that many
types involved, it is primarily an assembler representation
*/
typedef struct VValue VValue;
typedef enum
{
  v_nil,          // nil
  v_char_literal, // 'a'
  v_number,       // 0x00, 341, -123, etc
  v_error_type,   // error stuff
} VValueType;

/*all values are allocated on object memory pools*/
VValue *v_newValue (VObject *object);

/*create a value from a string*/
VValue *v_newValueFromString (VObject *object, char *str);

/* returns a string representation of the value type */
const char *v_valueTypeString (VValueType type);

/* checks if type is char literal */
int v_valueIsCharLiteral (VValue *value);

/* checks if type is a number*/
int v_valueIsNumber (VValue *value);

/*utility functions*/
VValueType v_valueType (VValue *value);
int v_valueIsNil (VValue *value);
int v_valueNumber (VValue *value);
char v_valueCharLiteral (VValue *value);

#endif // VVALUE_H
