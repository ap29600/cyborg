#ifndef COMPUTATION_H_
#define COMPUTATION_H_
#include "tablify.h"

static eval_state_t evaluate_node(table *T, size_t row, size_t col, FILE *stream);
static eval_state_t evaluate_expr_dependencies(table *T, string_view sv, FILE *stream);

static void calculate(table *T); 

static inline int is_expr(string_view sv) {
  return (sv.len && *sv.data == '=');
}

#endif // COMPUTATION_H_
