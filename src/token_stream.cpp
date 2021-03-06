#include "../include/token_stream.h"
#include "../include/command.h"
#include "../include/input_stream.h"
#include "../include/utils.h"

using namespace std;

TokenStream::TokenStream(char *input) {
  input_stream = InputStream(input);
  curr_token.type = null_token;
  curr_token.val = "";
}

TokenStream::TokenStream() {
  input_stream = InputStream();
}

token TokenStream::next() {
  token tok = curr_token;
  curr_token.type = null_token;
  curr_token.val = "";
  if (tok.type != null_token) {
    return tok;
  }
  line = input_stream.get_line();
  column = input_stream.get_column();
  read_while(is_whitespace, &input_stream);
  TokenCommand *command = tcd.dispatch(input_stream.peek(), &input_stream);
  curr_token = command->execute(&input_stream);
  return curr_token;
}

token TokenStream::peek() {
  if (curr_token.type != null_token) {
    return curr_token;
  }
  return next();
}

bool TokenStream::eof() {
  return input_stream.eof() && curr_token.type == null_token;
}

void TokenStream::croak(string msg, bool prev_tok_pos) {
  if (prev_tok_pos) {
    return input_stream.croak(msg, line, column);
  }
  return input_stream.croak(msg, input_stream.get_line(), input_stream.get_column());
}
