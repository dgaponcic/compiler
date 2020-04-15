#include "include/interpreter_print.h"
#include "include/parser.h"
#include "include/token_stream.h"
#include "include/utils.h"
#include <fstream>
#include <stdio.h>

using namespace std;

char *read_program(char program_name[]) {
  FILE *f = fopen(program_name, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *str = (char *)malloc(fsize + 1);
  fread(str, fsize, 1, f);
  fclose(f);

  str[fsize] = '\0';
  return str;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    exit(1);
  }

  char *program_name = argv[1];
  char *my_program = read_program(program_name);
  TokenStream token_stream = TokenStream(my_program);
  Parser parser = Parser(&token_stream);
  vector<Expression *> res = parser.parse();

  for (int i = 0; i < res.size(); i += 1) {
    print_ast(res[i], 0);
  }

  return 0;
}
