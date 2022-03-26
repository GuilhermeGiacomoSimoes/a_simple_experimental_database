#include "structure.h"

Row* b_tree_search(Page *page, uint32_t wanted_element);
void b_tree_insert(Page *root, Row *k);
Page* load_root();
void b_tree_insert_nonfull(Page *page, Row* k);
