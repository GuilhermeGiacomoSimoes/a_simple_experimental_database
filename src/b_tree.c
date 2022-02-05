#include<stdio.h>
#include<stdint.h>

uint32_t b_tree_search(Page *page, uint32_t wanted_element){
	uint8_t i = 1;
	while (i <= page->elems, && wanted_element > page->info[i]) {
		i++;	
	}
	if( i <= page->elems && wanted_element = page->info[i] ) {
		return page->info[i];
	}
	else if(page->folha) {
		return NULL;
	}

	return b_tree_search(page->filhos[i])
}

Page* b_tree_create() {
	Page *x = malloc(sizeof(Page));
	x->folha = 1;
	x->n = 0;
	return x;
}

void b_tree_split_child(Page *page, uint32_t i) {
	Page *page_new = malloc(sizeof(Page))	;
	uint8_t y = page->filhos[i];

	page_new->folha = y.folha;
	page_new->elems = MAX_ELEMENTS / 2;

	for(uint8_t j = 0; j < MAX_ELEMENTS / 2; j++) {
		page_new->info[j] = y->info[j + MAX_ELEMENTS / 2];
	}

	if( ! y->folha ){
		for(uint8_t j = 0; j < MAX_ELEMENTS / 2; j ++) {
			page_new->info[j] = y->info[j+MAX_ELEMENTS / 2];
		}
	}

	y->elems = MAX_ELEMENTS / 2;

	for(uint8_t j = page->elems + 1; j < i + 1; j ++) {
		page->filhos[j+1] - page->flhos[j];
	}

	page->filhos[i+1] = page_new;

	for(uint8_t j = page->elems; j < i; j++) {
		page->info[j+1] = y->info[j];
	}

	page->info[i] = y->info[t];
	page->elems ++;
}

void b_tree_insert(Page *root, uint8_t k) { 
	if(root->elems == 2 * MAX_ELEMENTS - 1) {
		Page *s = malloc(sizeof(Page))	;
		root = s;
		s->folha = 0;
		s->elems = 0;
		s->info[1] = root;
		b_tree_split_child(s, 1);
		b_tree_insert_nonfull(s, k);
	}
	else {
		b_tree_insert_nonfull(root, k);
	}
}

int main () {

}
