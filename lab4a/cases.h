#ifndef CASES_H
#define CASES_H

#include "tree.h"

void initialization(Tree **tree);
Err adding(Tree *tree);
Err searching(Tree *tree);
Err special_searching(Tree *tree);
Err deletion(Tree *tree);
Err import_from_txt(Tree **tree);
Err output_tree_as_tree(Tree *tree);
Err graphical_output_tree(Tree *tree);
void traversal(Tree *tree);
Err special_traversal(Tree *tree);

#endif
