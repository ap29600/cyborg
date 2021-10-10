#include "tablify.h"

static void ivec_insert(i_vector *V, int i, size_t pos) {
  if (V->len <= pos) {
    V->data = realloc(V->data, (pos + 1) * sizeof(int));
    memset(V->data + V->len, 0, pos + 1 - V->len);
    V->len = pos + 1;
  }
  V->data[pos] = i;
}

static void svec_insert(s_vector *V, string_view s, size_t pos) {
  if (V->len <= pos) {
    V->data = realloc(V->data, (pos + 1) * sizeof(string_view));
    memset(V->data + V->len, 0, pos + 1 - V->len);
    V->len = pos + 1;
  }
  V->data[pos] = s;
}

static void table_insert(table *T, cell token, size_t row, size_t col) {
  if (T->rows < row+1 || T->cols < col+1) {
    size_t new_rows = max(T->rows, row+1);
    size_t new_cols = max(T->cols, col+1);

    cell * new_matrix = calloc(new_rows * new_cols, sizeof(cell));
    for(size_t i = 0; i<T->rows; i++) {
      for(size_t j = 0; j<T->cols; j++) {
        new_matrix[(i * new_cols) + j] = *table_at(*T, i, j);
      }
    }
    if (T->matrix) free(T->matrix);
    *T = (table) {
      new_matrix, 
      new_rows, 
      new_cols,
    };
  }
  *table_at(*T, row, col) = token;
}
