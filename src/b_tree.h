#define MAX_ELEMENTS 1000;

typedef struct {
	int folha;
	uint16_t ordem;
	uint16_t elems;
	int info[MAX_VALUES];
	struct Page *filhos [MAX_VALUES + 1];
} Page;

uint32_t b_tree_search();
