#include "computation.h"
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void calculate(table *T) {

  FILE *stream = fopen("/tmp/tablify_cache", "w");
  
  fprintf(stream, 
          "import math as m\n"
          "import numpy as np\n"
          );


  for (size_t i = 0; i<T->rows; i++){
    for (size_t j = 0; j<T->cols; j++){
      cell *c = table_at(*T,i,j);
      if (is_expr(c->sv)) {
        c->sv = sv_trim(sv_split_escaped(&c->sv, SV("#")));
        evaluate_node(T, i, j, stream);
      }
    }
  }
  fclose(stream);

  FILE *python_out = popen("python < /tmp/tablify_cache", "r");
  string_view result = sv_from_stream(python_out);
  pclose(python_out);
  remove("/tmp/tablify_cache");
  
  while (result.len) {
    string_view line = sv_trim(sv_split(&result, SV("\n")));
    string_view cell_name = sv_trim(sv_split(&line, SV("=")));
    line = sv_trim(line);

    size_t col = cell_name.data[0] - 'A';
    size_t row = strtod(&cell_name.data[1], NULL);
    
    table_at(*T, row, col)->expr_result = line;
  }
}

static eval_state_t evaluate_node(table *T, size_t row, size_t col, FILE * stream) {
  cell *c = table_at(*T, row, col);
  switch (c->eval) {
    case UNSEEN:
      if (is_expr(c->sv)) {
        c->eval = PENDING;
        c->eval = evaluate_expr_dependencies(T, c->sv, stream);
        fprintf(stream, 
            "try:\n"
            "    %c%zu " SV_FMT "\n",
            (int)col + 'A', row, SV_ARG(c->sv));
        fprintf(stream, 
            "except:\n"
            "    %c%zu = \"?!?\"\n",
            (int)col + 'A', row);
        // we always print the result of expressions
        fprintf(stream, "print(\"%c%zu = \", %c%zu)\n", 
            (int)col + 'A', row ,
            (int)col + 'A', row );
      } else {
        fprintf(stream, 
            "try:\n"
            "    %c%zu = " SV_FMT "\n",
            (int)col + 'A', row, SV_ARG(c->sv));
        // but only print '?!?' if literals have errors in them.
        fprintf(stream, 
            "except:\n"
            "    %c%zu = \"?!?\"\n"
            "    print(\"%c%zu = ?!?\")\n", 
            (int)col + 'A', row ,
            (int)col + 'A', row );
        c->eval = DONE;
      }
      break;
    case PENDING:
      c->eval = UNDEFINED;
      break;
    case UNDEFINED:
    case DONE:
      break;
  }

  return c->eval;
}

static eval_state_t evaluate_expr_dependencies(table *T, string_view sv, FILE *stream) {
  int got_char = 0;
  size_t row;
  size_t col;
  eval_state_t result = DONE;

  for (size_t i = 0; i<sv.len; i++) {
    if (got_char && isdigit(sv.data[i])) {
      char * end_ptr = NULL;
      row = strtod(&sv.data[i], &end_ptr);
      if (end_ptr != &sv.data[i]) {
        eval_state_t e = evaluate_node(T, row, col, stream);
        if (e == UNDEFINED)
          result = UNDEFINED;
      } else got_char = 0;
    } else {
      got_char = 0;
      if (isupper(sv.data[i])) {
        got_char = 1;
        col = sv.data[i] - 'A';
      }
    }
  }
  return result;
}

