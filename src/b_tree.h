#ifndef _B_TREE_H_
#define _B_TREE_H_

Row* b_tree_search(Page *page, int wanted_element);
Page* load_root();
void b_tree_insert_nonfull(Page *page, Row* k);
void b_tree_insert(Page *root, Row *k);

#endif
