#include <stdio.h>
#include <fstream>
#include "include/token_stream.h"
#include "include/utils.h"
#include "include/parser.h"

using namespace std; 

// void print_struct(token curr_token) {
//   cout << curr_token.type << ' ' << curr_token.val << '\n';
// }

int main(int argc, char **argv) {
  // if (argc < 2) {
  //   exit(1);
  // }
  // char* program_name = argv[1];
  // char* my_program = read_program(program_name);
  // TokenStream token_stream(my_program);
  // while (!token_stream.eof()) {
  //   print_struct(token_stream.next());
  // }
  // return 0;

  if (argc < 2) {
    exit(1);
  }

  char* program_name = argv[1];
  char* my_program = read_program(program_name);
  TokenStream token_stream = TokenStream(my_program);

  // while (!token_stream.eof()) {
  //   print_tokens(token_stream.peek());
  //   token_stream.next();
  // }

  Parser parser = Parser(&token_stream);
  vector<Expression*> res = parser.parse();
  for (int i = 0; i < res.size(); i += 1) {
    print_ast(res[i]);
  }

  return 0;
}
