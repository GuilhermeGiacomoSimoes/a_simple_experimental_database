#include<stdio.h>
#include<stdint.h>

uint32_t b_tree_search(Page *page, uint32_t wanted_element){
	uint8_t i = 1;
	while (i <= page->elems && wanted_element > page->info[i]) {
		i++;	
	}
	if( i <= page->elems && wanted_element = page->info[i] ) {
		return page->info[i];
	}
	else if(page->folha) {
		return NULL;
	}
	//TODO fazer a leitura de uma nova página do disco
	//que no caso é a página page->childs[i]

	return b_tree_search(page->childs[i])
}

Page* b_tree_create() {
	Page *x = malloc(sizeof(Page));
	x->folha = 1;
	x->elems = 0;
	//TODO fazer a escrita dessa inicializacao no disco
	return x;
}

void b_tree_split_child(Page *parent_not_full, uint32_t index_child_full) {
	Page *page_new = malloc(sizeof(Page))	;
	Page *child_full = parent_not_full->childs[index_child_full];

	page_new->folha = child_full->folha;
	page_new->elems = child_full->elems - 1;

	for(int j = 0; j < child_full->elems - 1; j++) {
		page_new->info[j] = child_full->info[j + child_full->elems];
	}

	if(!child_full->folha){
		for(int j = 0; j < MAX_ELEMENTS; j ++) {
			page_new->info[j] = child_full->info[j+MAX_ELEMENTS / 2];
		}
	}

	child_full->elems = MAX_ELEMENTS;

	for(int j = parent_not_full->elems + 1; j < index_child_full + 1; j ++) {
		parent_not_full->childs[j+1] - parent_not_full->flhos[j];
	}

	parent_not_full->childs[index_child_full+1] = page_new;

	for(int j = parent_not_full->elems; j < index_child_full; j++) {
		parent_not_full->info[j+1] = child_full->info[j];
	}

	parent_not_full->info[index_child_full] = child_full->info[t];
	parent_not_full->elems ++;
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

void b_tree_insert_nonfull(Page *page, uint8_t k) {
	uint8_t i = page->elems;
	if(page->folha) {
		while(i >= 1 && k < page->info[i]) {
			page->info[i+1] = page->indo[i];
			i --;
		}
		page->info[i+1] = k;
		page->elems ++;
	}
	else {
		while(i >= 1 && k < page->info[i]) {
			i --;
		}
		i ++;
		if(page->childs[i]->elems = 1 * MAX_ELEMENTS / 2 - 1) {
			b_tree_split_child(page, i, page->childs[i]);
			if(k > page->info[i]) {
				i ++;
			}
		}
		b_tree_insert_nonfull(page->childs[i], k);
	}
}

int main () {

}
