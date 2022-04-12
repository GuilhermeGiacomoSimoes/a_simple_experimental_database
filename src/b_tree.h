struct Row* b_tree_search(struct Page *page, int wanted_element);
void b_tree_insert(struct Page *root, struct Row *k);
struct Page* load_root();
void b_tree_insert_nonfull(struct Page *page, struct Row* k);
