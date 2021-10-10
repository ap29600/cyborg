#include "../lib/stringview.c"
#include "tablify.h"
#include "printing.c"
#include "data.c"
#include "computation.c"
#include <stdio.h>



int main() {
  string_view input = sv_from_stream(stdin);
  table T = read_table(input);
  calculate(&T);
  print_table(T, stdout);
}

static table read_table(string_view input) {
  table result = {NULL, 0, 0};
  i_vector aligns = {0};

  string_view tmp = input;
  string_view line = {0};
  size_t row = 0;
  while(tmp.len) {
    line = sv_trim(sv_split(&tmp, SV("\n")));
    // throw away leading '|' and '+'
    if (line.len > 0 && sv_has(sep_line_delimiters, line.data[0]))
      line = sv_trim((string_view) {line.len - 1, line.data + 1});

    if (line.len > 0 && line.data[0] == '<')
        continue;

    if (is_separator(line)) {
      size_t col = 0;
      while (line.len) {
        string_view token = sv_trim(sv_split_escaped(&line, sep_line_delimiters));
        line = sv_trim(line);
        if(token.len) {
          align_t a = get_align(token);
          // store the alignment for this column
          ivec_insert(&aligns, a, col);
          // insert the separator at location
          table_insert(&result, (cell){ .sv = {0}, .align = a, .is_sep = 1 }, row, col);
          // we will check the first entry to check whether a row is a separator row.
          table_at(result, row, 0)->is_sep = 1;
        }
        col++;
      }
    } else {
      size_t col = 0;
      while (line.len) {
        string_view token = sv_trim(sv_split_escaped(&line, table_delimiter));
        line = sv_trim(line);
        if (token.len) {
          align_t a = DEDUCE;
          if (aligns.len > col) 
            a = aligns.data[col];
          table_insert(&result, (cell){ .sv = token, .align = a, .is_sep = 0 }, row, col);
        }
        col++;
      }
    }
    row++;
  }
  return result;
}

static int is_separator(string_view line) {
  int found = 0;
  for(size_t i = 0; i<line.len; i++) {
    if (!isspace(line.data[i]) && 
        !sv_has(separators, line.data[i]))
      return 0;
    else if (!found && sv_has(separators, line.data[i])) {
      found = 1;
    }
  }
  return found;
}

align_t get_align(string_view sv) {
  int marker_pos = -1;
  for(size_t i = 0; i<sv.len; i++) 
    if (sv.data[i] == ':') {
      marker_pos = i;
      break;
    }
  if (marker_pos >= 0) {
    float ratio = (float)marker_pos / (float)(sv.len - 1);
    // here there is a possibility for sv to have length 1.
    // this results in ratio = (0/0) = NAN.
    // in this case NAN > 0.5 returns false, and we align to the left.
    if (ratio > 0.5) 
      return RIGHT;
    return LEFT;
  } 
  return DEDUCE;
}
