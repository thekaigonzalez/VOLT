#include "VBuffer.h"
#include "VDefaults.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct VBuffer
{
  VObject *object; /* parent */
  char *ptr;       /* buffer */
  int size;        /* size */
  int capacity;    /* capacity */
};

VBuffer *
v_newBuffer (VObject *object)
{
  VBuffer *buffer = v_allocateMemory (v_objectPool (object), sizeof (VBuffer));
  buffer->object = object;

  if (!buffer)
    return NULL;

  buffer->ptr
      = v_allocateMemory (v_objectPool (object), VOLT_DEFAULT_INCREMENT + 1);

  if (!buffer->ptr)
    {
      return NULL;
    }

  buffer->size = 0;
  buffer->capacity = VOLT_DEFAULT_INCREMENT;

  return buffer;
}

void
v_appendBuffer (VBuffer *buffer, char data)
{
  if (!buffer)
    {
      return;
    }

  if ((buffer->size >= buffer->capacity)
      || (buffer->size + 1 >= buffer->capacity))
    {
      buffer->capacity = buffer->capacity * 2;
      buffer->ptr = v_reallocateMemory (v_objectPool (buffer->object),
                                        buffer->ptr, buffer->capacity + 1);
    }

  buffer->ptr[buffer->size] = data;
  buffer->ptr[buffer->size + 1] = 0;

  buffer->size++;
}

char *
v_copyBuffer (VBuffer *buffer)
{
  if (!buffer)
    {
      return NULL;
    }

  char *copy
      = v_allocateMemory (v_objectPool (buffer->object), buffer->size + 1);

  if (copy)
    {
      memcpy (copy, buffer->ptr, buffer->size + 1);
      // escape it
      copy[buffer->size] = 0;
      return copy;
    }

  return NULL;
}

void
v_clearBuffer (VBuffer *buffer)
{
  if (!buffer)
    {
      return;
    }

  memset (buffer->ptr, 0, buffer->size);

  buffer->size = 0;
  buffer->ptr[0] = 0;
}

int
v_bufferLength (VBuffer *buffer)
{
  if (buffer)
    {
      return buffer->size;
    }

  return 0;
}
