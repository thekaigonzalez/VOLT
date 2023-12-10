// $Id: VObject.h

#ifndef VOBJECT_H
#define VOBJECT_H

#include "VMemoryPool.h"

// Objects contain a memory pool and are essentially an environment for any
// types of memory to be allocated, each object holds it's own pool and is
// responsible for clearing it's own pool.
typedef struct VObject VObject;

#define v_error(object, message, ...)                                         \
  v_errorWithStackTrace (object, __func__, message, ##__VA_ARGS__)

VObject *v_newObject ();
VMemoryPool *v_objectPool (VObject *object);
int v_errorWithStackTrace (VObject *object, const char *from_function,
                           const char *message, ...);
void v_deleteObject (VObject *object);

#endif // POBJECT_H
