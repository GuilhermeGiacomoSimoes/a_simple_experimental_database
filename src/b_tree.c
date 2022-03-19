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
	int t = (MAX_ELEMENTS / 2) + 1;
	Page *page_new = malloc(sizeof(Page));
	Page *child_full = parent_not_full->childs[index_child_full];

	page_new->folha = child_full->folha;
	page_new->elems = t - 1;

	for(int j = 1; j <= t - 1; j++) {
		page_new->info[j] = child_full->info[j + t];
	}

	if(!child_full->folha){
		for(int j = 1; j <= t; j ++) {
			page_new->childs[j] = child_full->childs[j+t];
		}
	}

	child_full->elems = t - 1;
	for(int j = parent_not_full->elems + 1; j < index_child_full + 1; j++) {
		parent_not_full->childs[j+1] = parent_not_full->childs[j];
	}

	parent_not_full->childs[index_child_full+1] = page_new;

	for(int j = parent_not_full->elems; j < index_child_full + 1; j++) {
		parent_not_full->childs[j+1] = parent_not_full->childs[j];
	}

	parent_not_full->info[index_child_full] = child_full->info[parent_not_full->elems];
	parent_not_full->elems ++;
	//TODO escrever child_full no disco
	//escrever page_new no disco
	//escrever parent_not_full no disco
}

void b_tree_insert(Page *root, uint8_t k) { 
	if(root->elems == MAX_ELEMENTS) {
		Page *s = malloc(sizeof(Page))	;
		root = s;
		s->folha = 0;
		s->elems = 0;
		s->childs[1] = root;
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
		//TODO gravar page->childs[i] no disco

		if(page->childs[i]->elems == MAX_ELEMENTS) {
			b_tree_split_child(page, i);
			if(k > page->info[i]) {
				i ++;
			}
		}
		b_tree_insert_nonfull(page->childs[i], k);
	}
}

int main () {

}
