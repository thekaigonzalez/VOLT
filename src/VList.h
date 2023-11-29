// $Id: VList.h

#ifndef VLIST_H
#define VLIST_H

#include "VObject.h"
#include "VToken.h"
#include "VValue.h"

/* list */
typedef struct VList VList;

/* allocates a new list */
/* size(token) * default size */
VList *v_newList (VObject *object);

/* adds a token to the list */
void v_listAddToken (VList *list, VToken *token);

/*print all tokens*/
void v_listPrint (VList *list);

/*access to the list*/
VToken *v_listGetToken (VList *list, int index);
int v_listSize (VList *list);
int v_listCapacity (VList *list);

#endif // VLIST_H
