#include "../lib/stringview.h"
#include "../lib/args.h"

int sv_find_word(string_view sv, char *str) {
  int l = strlen(str);
  for (int i = 0; i < sv.len - l; i++) {
    char *p = &sv.data[i];

    // don't start matching in the middle of a word
    if (i > 0 && isalnum(p[-1]))
      continue;

    int match = 1;
    for (int j = 0; j < l; j++) {
      if (p[j] != str[j]) {
        match = 0;
        break;
      }
    }

    // don't accept a match that ends in the middle of a word
    if (match && (sv.len - l == i || !isalnum(p[l])))
      return i;
  }
  return -1;
}

string_view sv_first_word(string_view sv) {
  // consume up to beginning of first word.
  while (sv.len > 0 && isspace(*sv.data)) {
    sv.len--;
    sv.data++;
  }
  size_t new_len = 1;
  while (new_len < sv.len && !isspace(sv.data[new_len]))
    new_len++;
  sv.len = new_len;
  return sv;
};

char buf[1024];

typedef struct {
  string_view name;
  FILE *stream;
} file;

typedef struct {
  void *data;
  size_t el_size;
  size_t len;
  size_t capacity;
} vector;

void resize(vector *v, size_t capacity) {
  if (capacity > v->capacity) {
    v->capacity = v->capacity * 2 + 1;
    v->data = realloc(v->data, v->capacity * v->el_size);
  }
}

FILE *find_file(vector v, string_view fname) {
  for (int i = 0; i < v.len; i++)
    if (sv_cmp(fname, ((file *)v.data)[i].name) == 0)
      return ((file *)v.data)[i].stream;
  return NULL;
}

int main() {
  string_view input = sv_slurp_stream(stdin);
  string_view orig = input;

  FILE *curfile;

  vector v =
      (vector){.data = NULL, .el_size = sizeof(file), .len = 0, .capacity = 0};

  int tangling = 0;
  while (input.len > 0) {
    string_view line = sv_split(&input, '\n');
    if (sv_starts_with(line, "```")) {
      if (tangling)
        tangling = 0;
      else {
        int pos = sv_find_word(line, "file");
        if (pos >= 0) {
          tangling = 1;
          string_view rest_of_line =
              (string_view){.data = line.data + pos + 5, .len = line.len - pos};
          string_view fname = sv_first_word(rest_of_line);

          if (NULL == (curfile = find_file(v, fname))) {
              resize(&v, v.len + 1);
              ((file *)v.data)[v.len].name = fname;
              sprintf(buf, SV_FMT, SV_ARG(fname));
              curfile = fopen(buf, "w");
              ((file *)v.data)[v.len++].stream = curfile;
          }
        }
      }
    } else if (tangling) {
        fprintf(curfile, SV_FMT "\n", SV_ARG(line));
    }
  }

  for (size_t i = 0; i < v.len; i++) {
    printf("file : ((" SV_FMT "))\n", SV_ARG(((file *)v.data)[i].name));
    fclose(((file *)v.data)[i].stream);
  }

  free(orig.data);
}
