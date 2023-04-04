#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree)); // 트리 생성 p
  // TODO: initialize struct if needed
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t)); // nil 노드 생성
  nil_node->color = RBTREE_BLACK; // 모든 리프 노드는 검정색
  t -> nil = nil_node; 
  t -> root = nil_node; // 부모 노드도 nil을 가리켜야 함
  return t;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// 회전
// 좌회전
void left_rotation(rbtree *t, node_t *x) {
  node_t *y = x -> right; // y는 현재 x의 오른쪽 자식 노드임
  x -> right = y -> left; // y의 왼쪽 서브 트리를 먼저 떼서 x의 오른쪽 자식에 붙여줌
  if (y -> left != t -> nil) { // 떼고도 만약 y의 왼쪽이 nil이 아니라면
    y -> left -> parent = x; // y의 왼쪽 부모를 x로 변경 : 더 떼
  }
  y -> parent = x -> parent; // x의 부모를 y로 변경 : 회전시킴!
  // x의 위치 잡아주는 조건문 루트, 왼, 오 : 어쨌든 원래 x 자리에 y가 들어감
  if (x -> parent == t -> nil) { //x의 부모가 nil이라면 = x가 루트라면 : 회전했으니 루트를 바꾸자
    t -> root = y; // 트리의 루트를 y로 변경
  } else if (x == x -> parent -> left) { // 회전했는데 x가 x의 부모의 왼쪽자식이라면
    x -> parent -> left = y; //x의 부모의 왼쪽을 y로 변경
  } else { //x가 x의 부모의 오른쪽 자식이라면
    x -> parent -> right = y; //x의 부모의 오른쪽을 y로 변경
  }
  y -> left = x; //y의 왼쪽을 x로 변경
  x -> parent = y; //x의 부모를 y로 변경
  return; //리턴 값 없음 = void 선언
}

//우회전 = 좌회전 반대로
void right_rotation(rbtree *t, node_t *x) {
  node_t *y = x->left; //y는 현재 x의 왼쪽 자식 노드
  x -> left = y -> right; //오른쪽 떼서 왼쪽에 붙임
  if(y -> right != t -> nil) {
    y -> right -> parent = x;
  }
  y -> parent = x -> parent; //회전
  //x 위치 잡아주기
  if (x -> parent == t -> nil) {
    t -> root = y;
  } else if (x == x -> parent -> right) { //x가 right의 자식
    x -> parent -> right = y;
  } else {
    x -> parent -> left = y;
  }
  y -> right = x;
  x -> parent = y;
  return;
}

// 색 변경 함수
void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z -> parent -> color == RBTREE_RED) { //z의 부모가 red일때
  // Case # 1 현재 노드의 삼촌, 부모가 red, 할아범이 black : !회전 안 함!
    if (z -> parent == z -> parent -> parent -> left) { //z의 부모가 z의 조상의 왼쪽 자식이면
      node_t *uncle = z -> parent ->parent -> right; //z의 부모의 형제노드 = 삼촌
      if (uncle -> color == RBTREE_RED) { // 삼촌이 red라면
        z -> parent -> color = RBTREE_BLACK; // 부모 red > black
        uncle -> color = RBTREE_BLACK; // 삼촌 red > black
        z -> parent -> parent -> color = RBTREE_RED; // 할아범 black > red
        z = z -> parent -> parent; // 할아버지 가리켜야 다시 while문을 돈다!
      } else { // 삼촌이 black Case 2, 3
        if (z == z -> parent ->right) { //꺾새 모양
          z = z -> parent;
          left_rotation(t, z); // 좌회전
        } // Case 3이 됨 > 색 바꿔주고 우회전하면 끝남
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotation(t, z -> parent -> parent); // 조상 기준으로 우회전해야함
      }
    } else { //z의 부모가 z의 조상의 오른쪽이면 위의 과정을 반대로 하면 됨
      node_t *uncle = z -> parent -> parent -> left;
      if (uncle -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        uncle -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      } else {
        if (z == z -> parent -> left) {
          z = z -> parent;
          right_rotation(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotation(t, z -> parent ->parent);
      }
    }
  } //While문 끝
  t -> root -> color = RBTREE_BLACK; // 규칙 2를 위해서 root는 항상 black
  return; //void형
}

// 데이터 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t -> nil; // y = 트리의 nil 노드
  node_t *x = t -> root; // x = 트리의 root
  
  while (x != t -> nil) { //x가 nil에 도달할때까지 반복
    y = x;
    if (key < x->key) { //만약 x의 key값보다 삽입할 key의 값이 작으면
      x = x -> left; //x를 x의 왼쪽으로 변경
    } else { //만약 x의 key값보다 삽입할 key값이 크거나 같으면
      x = x -> right;  // x를 x의 오른쪽으로 변경
    }
  }
  //x가 nil을 가리킴 > z를 삽입할 시기
  node_t *z = (node_t *)calloc(1, sizeof(node_t)); // z 노드 생성
  z -> key = key; // z의 key값 넣어줌

  z -> parent = y; // z의 부모는 y
  if (y == t -> nil) { //y가 트리의 nil일때, 비어있던 애(첫 노드)
    t -> root = z; // Case 1 (고려할 필요 없는 경우임 걍 들어감)
  } else if (z -> key < y -> key) { // y의 key값이 삽입할 key값보다 작을 때
    y -> left = z; // y의 왼쪽 자식으로 감
  } else {
    y -> right = z;
  }
  
  z -> left = t -> nil; // z 왼쪽 자식 nil
  z -> right = t -> nil; // z 오른쪽 자식 nil
  z -> color = RBTREE_RED; // 삽입은 늘 red
  // left_rotation(t,z);
  // if (z->parent->parent != NULL) {
  //   right_rotation(t,z);
  // }
  
  rbtree_insert_fixup(t, z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

// 트리의 최소값 = BST특성
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t -> root; //x는 트리의 루트
  while (x -> left != t -> nil) { //x의 왼쪽 자식이 nil이 아닐때
    x = x -> left; // 왼쪽이 제일 작은 값임
  }
  return t->root;
}

// 트리의 최대값 = BST특성
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t -> root; //x는 트리의 루트
  while (x -> right != t -> nil) { //x의 왼쪽 자식이 nil이 아닐때
    x = x -> right; // 왼쪽이 제일 작은 값임
  }
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
