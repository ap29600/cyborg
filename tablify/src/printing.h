#ifndef PRINTING_H_
#define PRINTING_H_

#include "../lib/stringview.h"
#include "data.h"

static void fprint_padded(FILE *stream, cell c, size_t width);
static void print_table(table T, FILE *stream);

#endif // PRINTING_H_
