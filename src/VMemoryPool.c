#include "VMemoryPool.h"

#include <stdio.h>
#include <string.h>

struct _MemoryPool
{
  void **ptr;
  int *ptr_sizes;

  int hardLimit;

  int capacity;
  int size;

  _Bool U_warnForBigRegisters;
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
  pool->ptr_sizes = calloc (softLimit, sizeof (int));

  pool->hardLimit = hardLimit;

  if (!pool->ptr)
    {
      free (pool);
      return NULL;
    }

  pool->U_warnForBigRegisters = true;

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
  free (pool->ptr_sizes);
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
      pool->ptr_sizes
          = realloc (pool->ptr_sizes, pool->capacity * sizeof (int));

      if (!pool->ptr || !pool->ptr_sizes)
        {
          return NULL;
        }
    }

  void *memn = malloc (size);

  pool->ptr_sizes[pool->size] = size;

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

int
v_memoryPoolSizeof (VMemoryPool *pool, int index)
{
  if (pool)
    {
      if (index >= 0 && index < pool->size)
        {
          return pool->ptr_sizes[index];
        }
    }
  return 0;
}

void *
v_memoryPoolAt (VMemoryPool *pool, int index)
{
  if (pool)
    {
      if (index >= 0 && index < pool->size)
        {
          return pool->ptr[index];
        }
    }

  return NULL;
}

_Bool
v_W_warnForBigRegisters (VMemoryPool *pool)
{
  if (pool)
    {
      return pool->U_warnForBigRegisters;
    }
  return false;
}

void
v_W_setWarnForBigRegisters (VMemoryPool *pool, _Bool value)
{
  if (pool)
    {
      pool->U_warnForBigRegisters = value;
    }
}
