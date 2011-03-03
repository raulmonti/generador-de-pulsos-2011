#ifndef _GLIST_H_
#define _GLIST_H_

typedef struct _GList	GList;

struct _GList
{
  void *data;
  GList *next;
  GList *prev;
};

void g_list_free(GList *list);
GList* g_list_append(GList *list, void* data);
GList* g_list_remove(GList *list, const void* data);
GList* g_list_remove_link(GList *list, GList *llink);
GList* g_list_nth(GList *list, unsigned int n);
GList* g_list_first(GList *list);
unsigned int g_list_length(GList *list);
void* g_list_nth_data(GList *list, unsigned int n);
GList* g_list_next(GList *list);
GList* g_list_previous(GList *list);
GList* g_list_last(GList *list);
GList* g_list_prepend(GList *list , void* data);
GList* g_list_insert(GList *list , void* data, unsigned int from);

#endif 
