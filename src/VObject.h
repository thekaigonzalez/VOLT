// $Id: VObject.h

#ifndef VOBJECT_H
#define VOBJECT_H

#include "VMemoryPool.h"

// Objects contain a memory pool and are essentially an environment for any
// types of memory to be allocated, each object holds it's own pool and is
// responsible for clearing it's own pool.
typedef struct VObject VObject;

VObject *v_newObject ();
VMemoryPool *v_objectPool (VObject *object);

void v_deleteObject (VObject *object);

#endif // POBJECT_H
