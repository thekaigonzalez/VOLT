// $Id: VBuffer.h

#ifndef VBUFFER_H
#define VBUFFER_H

#include "VObject.h"

typedef struct VBuffer VBuffer;

VBuffer *v_newBuffer (VObject *object);
void v_appendBuffer (VBuffer *buffer, char data);
char *v_copyBuffer (VBuffer *buffer);
void v_clearBuffer (VBuffer *buffer);
int v_bufferLength (VBuffer *buffer);

#endif // VBUFFER_H
