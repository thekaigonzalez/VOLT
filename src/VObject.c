#include "VObject.h"
#include "VDefaults.h"

#include <stdio.h>

struct VObject
{
  VMemoryPool *pool;
};

VObject *
v_newObject ()
{
  VObject *object = malloc (sizeof (VObject));

  if (!object)
    {
      return NULL;
    }

  object->pool
      = v_newMemoryPool (VOLT_DEFAULT_HARD_LIMIT, VOLT_DEFAULT_INCREMENT);

  if (!object->pool)
    {
      printf ("warn: v_newObject could not allocate memory pool\n");
      free (object);
      object = NULL;
    }

  return object;
}

VMemoryPool *
v_objectPool (VObject *object)
{
  if (object && object->pool)
    {
      return object->pool;
    }
}

void
v_deleteObject (VObject *object)
{
  if (object)
    {
      v_deleteMemoryPool (object->pool);
      free (object);
      object = NULL;
    }
}
