#include "tablify.h"
#include "printing.h"

static void print_table(table T, FILE *stream) {
  s_vector delay = {0};

  size_t widths[T.cols];
  for(size_t j = 0; j < T.cols; j++) {
    widths[j] = 0;
    for(size_t i = 0; i < T.rows; i++) {
      cell * c = table_at(T, i, j);
      size_t entry_len = sv_len_utf_8(c->sv);
      if (c->expr_result.len > 0) {
        c->result_len_utf_8 = sv_len_utf_8(c->expr_result);

        if (c->result_len_utf_8 > 20) {
            svec_insert(&delay, c->expr_result, delay.len);
            char * placeholder = malloc(32);
            c->result_len_utf_8 = snprintf(placeholder, 32, "<%zu>", delay.len);
            c->expr_result = sv_from_cstr(placeholder);
        }
        entry_len += 3 + c->result_len_utf_8;
      }
      if (entry_len > widths[j])
        widths[j] = entry_len;
    }
  }


  for(size_t i = 0; i < T.rows; i++) {
    int sep = table_at(T, i, 0)->is_sep;
    char delim = sep ? '+' : '|'; 
    if (sep) {
      for(size_t j = 0; j < T.cols; j++){
        table_at(T,i,j)->is_sep = 1;
      }
    }

    fprintf(stream, "%c", delim);
    for(size_t j = 0; j < T.cols; j++){
      fprint_padded(stream, *table_at(T, i, j), widths[j] + 2);
      fprintf(stream, "%c", delim);
    }
    fprintf(stream, "\n");
  }

  for (size_t i = 0 ; i < delay.len; i++) {
    fprintf(stream, "<%zu> = " SV_FMT "\n", i+1, SV_ARG(delay.data[i]));
  }
}

static void fprint_padded(FILE *stream, cell c, size_t width) {
  if (c.is_sep) {
    if (c.align == LEFT) fputc(':', stream);
    for(size_t i = 0; i < width - (c.align > 0); i++) fputc('-', stream);
    if (c.align == RIGHT) fputc(':', stream);
  } else {
    size_t len = sv_len_utf_8(c.sv);
    if (c.result_len_utf_8 > 0) {
      len += 3 + c.result_len_utf_8;
    }
    if (len > width) {
      fprintf(stderr, 
          "ERROR: attempting to print sv:\"" SV_FMT 
          "\" of length %zu, at width %zu\n",
          SV_ARG(c.sv), c.sv.len, width);
      exit(1);
    }

    size_t pad_left;
    size_t pad_right;

    switch (c.align) {
      case RIGHT:
        pad_right = 1;
        pad_left = width - len - pad_right;
        break;
      case LEFT:
        pad_left = 1;
        pad_right = width - len - pad_left;
        break;
      case DEDUCE:
      default:
        pad_left = (width - len) / 2;
        pad_right = width - len - pad_left;
        break;
    }

    for (size_t i = 0; i < pad_left; i++) fputc(' ', stream);
    fprintf(stream, SV_FMT, SV_ARG(c.sv));
    if (c.result_len_utf_8 > 0) {
      fprintf(stream, " # " SV_FMT, SV_ARG(c.expr_result));
    }
    for (size_t i = 0; i < pad_right; i++) fputc(' ', stream);
  }
}
