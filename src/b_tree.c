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

int main () {

}
