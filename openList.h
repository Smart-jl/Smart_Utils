#ifndef __OPEN_LIST_H
#define __OPEN_LIST_H

/* Internal function */
#define ol_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

 /**
   * @brief  Page List get struct entry address
   * @param  node: the address of entry node
   * @param  type: struct type
   * @param  member: the name of node in struct
   * @retval None
   */
#define ol_ListEntry(node, type, member) \
    ol_container_of(node, type, member)

typedef struct s_openList{
	struct s_openList* next;
	struct s_openList* prev;
}openList;

typedef struct {
	void (*Init)(openList* l);
	void (*InsertAfter)(openList* l, openList* n);
	void (*InsertBefore)(openList* l, openList* n);
    void (*InsertLast)(openList* title, openList* n);
	void (*Remove)(openList* l);
}openListController;

#ifdef __cplusplus
extern "C"{
#endif
openListController* Ol_Administration(void);
#ifdef __cplusplus
}
#endif
#endif 
