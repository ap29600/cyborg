#ifndef TABLIFY_H_
#define TABLIFY_H_

#include "../lib/stringview.h"
#include "data.h"


static string_view table_delimiter = SV("|");
static string_view separators = SV("|+=-:");
static string_view sep_line_delimiters = SV("+|");

static table read_table(string_view input);
static int is_separator(string_view line);

static inline size_t max(size_t a, size_t b) {
    return a > b ? a : b ; 
}

static align_t get_align(string_view sv);

#endif // TABLIFY_H_
