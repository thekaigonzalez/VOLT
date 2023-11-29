#include "VMemoryPool.h"

#include <stdio.h>
#include <string.h>

struct _MemoryPool
{
  void **ptr;

  int hardLimit;

  int capacity;
  int size;
};

VMemoryPool *
v_newMemoryPool (int hardLimit, int softLimit)
{
  VMemoryPool *pool = calloc (1, sizeof (VMemoryPool));

  if (!pool)
    {
      return NULL;
    }
  pool->size = 0;
  pool->capacity = softLimit;
  pool->ptr = calloc (softLimit, sizeof (void *));

  pool->hardLimit = hardLimit;

  if (!pool->ptr)
    {
      free (pool);
      return NULL;
    }

  return pool;
}

void
v_appendMemoryPool (VMemoryPool *p, void *ptr)
{
  if (!p || !ptr)
    return;
  p->ptr[p->size] = ptr;
  p->size++;
}

void
v_deleteMemoryPool (VMemoryPool *pool)
{
  if (!pool)
    return;

  for (int i = 0; i < pool->size; i++)
    {
      free (pool->ptr[i]);
      pool->ptr[i] = NULL; // Set the pointer to NULL after freeing
    }

  free (pool->ptr);
  free (pool);
}

void *
v_allocateMemory (VMemoryPool *pool, int size)
{
  if (pool->size >= pool->hardLimit)
    {
      return NULL;
    }

  if (pool->size >= pool->capacity)
    {
      pool->capacity *= 2;
      pool->ptr = realloc (pool->ptr, pool->capacity * sizeof (void *));
    }

  void *memn = malloc (size);

  v_appendMemoryPool (pool, memn);

  return memn;
}

void *
v_reallocateMemory (VMemoryPool *pool, void *ptr, int size)
{
  if (!pool)
    return NULL;

  for (int i = 0; i < pool->size; i++)
    {
      if (pool->ptr[i] == ptr)
        {
          void *memory = realloc (ptr, size);

          if (memory)
            {
              pool->ptr[i] = memory;
              return memory;
            }
          return NULL;
        }
    }

  void *memory = malloc (size);
  if (memory)
    {
      v_appendMemoryPool (pool, memory);
      return memory;
    }

  return NULL;
}

int
v_memoryPoolSize (VMemoryPool *pool)
{
  if (pool)
    return pool->size;
  return -1;
}

int
v_memoryPoolCapacity (VMemoryPool *pool)
{
  if (pool)
    return pool->capacity;
  return -1;
}

int
v_memoryPoolIncrement (VMemoryPool *pool)
{
  if (pool)
    return pool->capacity;
  return -1;
}

int
v_memoryPoolHardLimit (VMemoryPool *pool)
{
  if (pool)
    return pool->hardLimit;
  return -1;
}
