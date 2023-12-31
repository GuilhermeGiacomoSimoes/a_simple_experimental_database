#ifndef __TREE_H__
#define __TREE_H__

#include<stdarg.h>

#include "structure.h"

Row* search(Page *page, int wanted_element, ...);
Page* load_root();
Page* create();
int insert(Page *root, Row *k, ...);

#endif