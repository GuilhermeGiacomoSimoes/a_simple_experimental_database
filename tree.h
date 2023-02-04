#ifndef __TREE_H__
#define __TREE_H__

Row* search(Page *page, int wanted_element);
Page* load_root();
Page* create();
void insert(Page *root, Row *k);

#endif