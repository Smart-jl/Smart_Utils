#include "openList.h"

/**
  * @brief  Page List insert after
  * @param  l: base list
  * @param  n: want to insert
  * @retval None
  */
static void _insert_after(openList *l, openList *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}
/**
  * @brief  Page List insert before
  * @param  l: base list
  * @param  n: want to insert
  * @retval None
  */
static void _insert_before(openList *l, openList *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}
/**
  * @brief  Page List remove
  * @param  n: want to remove
  * @retval None
  */
static void _remove(openList *n){
	n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}
/**
  * @brief  Page List init
  * @param  n: want to init
  * @retval None
  */
static void  _init(openList *l){
	l->next = l->prev = l ;
}

static void _insert_last(openList *title, openList *n){
  openList* list_temp = title;
	while (list_temp->next != title) {
		list_temp = list_temp->next;
	}
	_insert_after(list_temp, n);
}

static openListController list_controller = {
    _init,
    _insert_after,
    _insert_before,
    _insert_last,
    _remove
};
/**
  * @brief  Page List get administration
  * @param  None
  * @retval the pointer of administration
  */
openListController* Ol_Administration(void) {
    return &list_controller;
}

