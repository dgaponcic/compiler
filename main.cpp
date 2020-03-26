#include <stdio.h>
#include <fstream>
#include "include/token_stream.h"
#include "include/utils.h"
#include "include/parser.h"
#include "include/interpreter_print.h"

using namespace std; 

int main(int argc, char **argv) {
  if (argc < 2) {
    exit(1);
  }

  char* program_name = argv[1];
  char* my_program = read_program(program_name);
  TokenStream token_stream = TokenStream(my_program);
  Parser parser = Parser(&token_stream);
  vector<Expression*> res = parser.parse();
  
  for (int i = 0; i < res.size(); i += 1) {
    print_ast(res[i], 0);
  }
  
  return 0;
}
