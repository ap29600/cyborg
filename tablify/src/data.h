#ifndef DATA_H_
#define DATA_H_

#include "../lib/stringview.h"

typedef enum {
  DEDUCE= 0,
  LEFT,
  RIGHT,
} align_t;

typedef enum {
  UNSEEN = 0,
  PENDING,
  DONE,
  UNDEFINED,
} eval_state_t;

typedef struct {
    string_view sv;
    string_view expr_result;
    size_t result_len_utf_8;
    eval_state_t eval;
    align_t align;
    int is_sep;
} cell;

////

typedef struct {
  cell *matrix;
  size_t rows;
  size_t cols;
} table;

static void table_insert(table *T, cell token, size_t row, size_t col);

static inline cell *table_at(table T, size_t row, size_t col) {
    return &T.matrix[row * T.cols + col];
}

////

typedef struct {
  int *data;
  size_t len;
} i_vector;

static void ivec_insert(i_vector *V, int i, size_t pos);

typedef struct {
  string_view *data;
  size_t len;
} s_vector;

static void svec_insert(s_vector *V, string_view s, size_t pos);

#endif // DATA_H_
