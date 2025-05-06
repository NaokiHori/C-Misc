#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_t {
  char data;
  struct node_t * next;
} node_t;

static int check_nodes(
    const node_t * root_node
) {
  printf("inspect list: ");
  while (root_node) {
    printf("%c ", root_node->data);
    root_node = root_node->next;
  }
  puts("");
  return 0;
}

static int add_node(
    node_t ** const root_node,
    const char data
) {
  node_t * new_node = malloc(sizeof(node_t));
  new_node->data = data;
  // the new node comes to the head (root node)
  new_node->next = *root_node;
  *root_node = new_node;
  return 0;
}

static int remove_node(
    node_t ** root_node,
    const char data
) {
  while (*root_node) {
    if (data == (*root_node)->data) {
      node_t * const next = (*root_node)->next;
      free(*root_node);
      *root_node = next;
      // assume at most one item is present
      return 0;
    }
    root_node = &(*root_node)->next;
  }
  // not found
  return 1;
}

int main(
    void
) {
  node_t * root_node = NULL;
  check_nodes(root_node);
  // add some items to the list
  add_node(&root_node, 'd');
  check_nodes(root_node);
  add_node(&root_node, 'c');
  check_nodes(root_node);
  add_node(&root_node, 'b');
  check_nodes(root_node);
  add_node(&root_node, 'a');
  check_nodes(root_node);
  // remove elements
  // from middle
  remove_node(&root_node, 'b');
  check_nodes(root_node);
  // from tail
  remove_node(&root_node, 'd');
  check_nodes(root_node);
  // from head
  remove_node(&root_node, 'a');
  check_nodes(root_node);
  // head and tail (only one item exists)
  remove_node(&root_node, 'c');
  check_nodes(root_node);
  return 0;
}

