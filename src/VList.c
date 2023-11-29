#include "VList.h"
#include "VDefaults.h"

struct VList
{
  VObject *object; /* parent */
  VToken **data;   /* data */
  int size;        /* size */
  int capacity;    /* capacity */
};

VList *
v_newList (VObject *object)
{
  if (!object)
    {
      return NULL;
    }

  VList *v = v_allocateMemory (v_objectPool (object), sizeof (VList));
  if (v)
    {
      v->object = object;
      v->data = v_allocateMemory (v_objectPool (object),
                                  sizeof (VToken *) * VOLT_DEFAULT_INCREMENT);
      v->size = 0;
      v->capacity = VOLT_DEFAULT_INCREMENT;

      if (!v->data)
        {
          return NULL;
        }

      return v;
    }
}

void
v_listAddToken (VList *list, VToken *token)
{
  if (!list || !token)
    {
      return;
    }

  if (list->size >= list->capacity)
    {
      list->capacity *= 2;
      list->data = v_reallocateMemory (v_objectPool (list->object), list->data,
                                       sizeof (VToken *) * list->capacity);
    }

  list->data[list->size] = token;
  list->size++;
}

void
v_listPrint (VList *list)
{
  if (!list)
    {
      return;
    }

  for (int i = 0; i < list->size; i++)
    {
      v_tokenPrint (list->data[i]);
    }
}

VToken *
v_listGetToken (VList *list, int index)
{
  if (!list)
    {
      return NULL;
    }
  if (index < 0 || index >= list->size)
    {
      return NULL;
    }

  return list->data[index];
}

int
v_listSize (VList *list)
{
  if (!list)
    {
      return 0;
    }
  return list->size;
}
