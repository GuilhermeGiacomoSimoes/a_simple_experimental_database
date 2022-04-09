Row* b_tree_search(Page *page, int wanted_element);
void b_tree_insert(Page *root, Row *k);
Page* load_root();
void b_tree_insert_nonfull(Page *page, Row* k);
