// $Id: PMemoryPool.h

#ifndef VMEMORYPOOL_H
#define VMEMORYPOOL_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef struct _MemoryPool VMemoryPool;

  /* dont get confused, softlimit is essentially just capacity, the increment
  is how much memory is allocated for each append, depending on if the size
  goes over the cap */
  VMemoryPool *v_newMemoryPool (int hardLimit, int softLimit);
  void v_appendMemoryPool (VMemoryPool *pool, void *ptr);
  void v_deleteMemoryPool (VMemoryPool *pool);

  void *v_allocateMemory (VMemoryPool *pool, int size);
  void *v_reallocateMemory (VMemoryPool *pool, void *ptr, int size);

  /*memory pool utility functions*/
  int v_memoryPoolSize (VMemoryPool *pool);
  int v_memoryPoolCapacity (VMemoryPool *pool);
  int v_memoryPoolIncrement (VMemoryPool *pool);
  int v_memoryPoolHardLimit (VMemoryPool *pool);

#ifdef __cplusplus
}
#endif

#endif // MemoryPool_H
