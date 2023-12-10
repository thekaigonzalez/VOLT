#include "VObject.h"
#include "VDefaults.h"
#include "VMemoryPool.h"

#include <stdarg.h>
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

  return NULL;
}
int
v_errorWithStackTrace (VObject *object, const char *from_function,
                       const char *message, ...)
{
  if (object)
    {
      va_list args;
      va_start (args, message);

      printf ("  Compiler Error (Called from %s)\n", from_function);
      printf ("  Message          | ");
      fflush (stdout);
      vfprintf (stderr, message, args);
      fflush (stderr);
      printf ("\n");
      printf ("  Memory Pool Cap. | %d\n",
              v_memoryPoolCapacity (object->pool));
      printf ("  Memory Pool Size | %d\n", v_memoryPoolSize (object->pool));

      printf ("Stack Trace:\n");
      printf ("  ==============================\n");

      for (int i = 0; i < v_memoryPoolSize (object->pool); i++)
        {
          if (!v_memoryPoolAt (object->pool, i))
            {
              printf ("    %d | (nonexistent)\n", i);
            }
          else
            {
              printf ("    instance %d asked for heap size of `%d' (%p)\n", i,
                      v_memoryPoolSizeof (object->pool, i),
                      v_memoryPoolAt (object->pool, i));
            }
        }

      va_end (args);

      v_deleteMemoryPool (object->pool);

      exit (EXIT_FAILURE);
    }
  exit (EXIT_FAILURE);
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
