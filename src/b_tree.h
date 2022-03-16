#define MAX_ELEMENTS 1000;

typedef struct {
	int folha;
	int ordem;
	int elems;
	int info[MAX_VALUES];
	struct Page *childs [MAX_VALUES + 1];
} Page;

uint32_t b_tree_search(Page *page, uint32_t wanted_element);
