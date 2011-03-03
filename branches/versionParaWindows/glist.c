#include "glist.h"
#include <stdlib.h>
#include <assert.h>

void g_list_free(GList *list){
    GList* aux = NULL;

    while (list != NULL){
        aux = list;
        list = list->next;
        free(aux);
    }
}
GList* g_list_append(GList *list, void* data){
    GList* aux = NULL;

    if(list != NULL){
        aux = list;

        while (aux->next != NULL){
            aux = aux->next;
        }

        aux->next = calloc(1, sizeof(struct _GList));
        aux->next->next = NULL;
        aux->next->data = data;
        aux->next->prev = aux->next;
    }else{
        list = calloc(1, sizeof(struct _GList));
        list->next = NULL;
        list->data = data;
        list->prev = NULL;
    }
    return list;
}
GList* g_list_remove(GList *list, const void* data){
    GList *aux = NULL;
    GList *killer = NULL;
    GList *result = NULL;

    if(list != NULL){
        aux = g_list_first(list);
        if(aux->data == data){
            result = aux->next;
            result->prev = NULL;
            free(aux);
        }else{
            result = list;
            while(aux->next != NULL ){
                if(aux->next->data == data){
                    killer = aux->next;
                    aux->next = aux->next->next;
                    aux->next->prev = aux;
                    free(killer);
                    break;
                }
                aux = aux->next;
            }
        }
    }

    return result;

}
GList* g_list_remove_link(GList *list, GList *llink){
    GList *aux = NULL;
    GList *result = NULL;

    assert(llink != NULL);

    if(llink == list){
      result = list->next;
    }else{
      result = list;
    }

    if(llink->prev != NULL){
        llink->prev->next = llink->next;
    }
    if(llink->next != NULL){
        llink->next->prev = llink->prev;
    }
    llink->next = NULL;
    llink->prev = NULL;

    return result;
}
GList* g_list_nth(GList *list, unsigned int n){
    unsigned int i = 0;
    GList *aux = NULL;

    aux = g_list_first(list);
    for(i=0;i<n;i++){
        aux = aux->next;
    }
    return aux;
}

GList* g_list_first(GList *list){
    GList *aux = NULL;
    GList *result = NULL;

    if(list != NULL){
        aux = list;
        while (aux->prev != NULL){
            aux = aux->prev;
        }
        result = aux;
    }
    return result;
}
unsigned int g_list_length(GList *list){
    unsigned int count = 0;
    GList *aux = NULL;

    if (list != NULL){
        count = 1;
        aux = g_list_first(list);
        while (aux != NULL){
            aux = aux->next;
            count++;
        }
    }
    return count;
}
void* g_list_nth_data(GList *list, unsigned int n){
    unsigned int i = 0;
    GList *aux = NULL;

    aux = g_list_first(list);
    for(i=0;i<n;i++){
        aux = aux->next;
    }
    return aux->data;
}

GList* g_list_next(GList *list){
    assert(list != list);

    return list->next;
}

GList* g_list_previous(GList *list){
    assert(list != list);

    return list->prev;
}

GList* g_list_last(GList *list){
    assert(list != NULL);
    while (list->next != NULL){
        list = list->next;
    }
    return list;
}

GList* g_list_prepend(GList *list , void* data){
    GList* aux = calloc(1, sizeof(struct _GList));
    if(list != NULL){
        list = g_list_first(list);
        list->prev = aux;
        aux->next = list;
        aux->prev = NULL;
        aux->data = data;
    }
    return aux;
}

GList* g_list_insert(GList *list , void* data, unsigned int from){
    unsigned int i = 0;
    GList *aux = NULL;

    assert(list != NULL);

    list = g_list_first(list);
    aux = calloc(1, sizeof(struct _GList));
    aux->data = data;

    if(from > g_list_length(list)){
      list = g_list_append(list, data);
    }else if(from == 0){
      list->prev = aux;
      aux->next = list;
      aux->prev = NULL;
    }else{
      for(i=1;i<from;i++){
        list = list->next;
      }
      aux->next = list->next;
      aux->prev = list;
      list->next->prev = aux;
      list->next = aux;
    }

    return g_list_first(list);
}
