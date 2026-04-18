#ifndef CASES_H
#define CASES_H

#include "table_.h"

void initialization(Table **table);
Err adding(Table *table);
Err finding_certain_release(Table *table);
Err finding_all_release(Table *table);
Err deleting_certain_release(Table *table);
Err deleting_all_release(Table *table);
Err import_from_txt(Table **table);
void printing(Table *table);

#endif
