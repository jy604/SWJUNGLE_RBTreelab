#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t; //색 지정 red or black

typedef int key_t; 

typedef struct node_t {
  color_t color; //노드 색
  key_t key; //노드 값
  struct node_t *parent, *left, *right; //노드 부모, 왼쪽 자식, 오른쪽 자식
} node_t;

typedef struct {
  node_t *root; //루트 노드
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
