#include "../include/utils.h"
#include "../include/command.h"
#include "../include/input_stream.h"

using namespace std; 

token EOFCommand::execute(InputStream* input_stream) {
  curr_token.type = null_token;
  curr_token.val = "";

  return curr_token;
};

token OperatorCommand::execute(InputStream* input_stream) {
  curr_token.type = op;
  curr_token.val = read_while(is_operator, input_stream);
  return curr_token;
};

token PunctuationCommand::execute(InputStream* input_stream) {
  curr_token.type = punct;
  curr_token.val = input_stream->next();
  return curr_token;
};

token StringCommand::execute(InputStream* input_stream) {
  input_stream->next();
  curr_token.type = str;
  curr_token.val = read_while(is_string_char, input_stream);
  input_stream->next();
  return curr_token;
};

token AlphaCommand::execute(InputStream* input_stream) {
  curr_token.val = read_while(is_alpha_num_symb, input_stream);
  if (is_keyword(curr_token.val)) {
    curr_token.type = keyword;
  } else if(is_bool_operator(curr_token.val)){
    curr_token.type = op;
  } else if(is_boolean(curr_token.val)){
    curr_token.type = boolean;
  } else {
    curr_token.type = var;
  }
  return curr_token;
};

token NullTokenCommand::execute(InputStream* input_stream) {
  curr_token.val = "";
  curr_token.type = null_token;
  return curr_token;
}

token NumberCommand::execute(InputStream* input_stream) {
  string num = "";
  token curr_token;
  curr_token.type = number;
  bool has_dot = false;
  while(is_digit(input_stream->peek()) || (input_stream->peek() == '.' && !has_dot)) {
    num += input_stream->peek();
    if (input_stream->peek() == '.') {
      has_dot = true;
    }
    input_stream->next();
  }
  curr_token.val = num;
  return curr_token;
}

TokenCommand* TokenCommandDispatcher::dispatch(char chr, InputStream* input_stream) {
  if (is_whitespace(chr) || input_stream->eof()) {
    return new EOFCommand();
  }

  else if (is_operator(chr)) {
    return new OperatorCommand();
  }

  else if(is_punctuation(chr)) {
    return new PunctuationCommand();
  }

  else if (is_string_delimitator(chr)) {
    return new StringCommand();
  }

  else if(isalpha(chr)) {
    return new AlphaCommand();
  }

  else if(isdigit(chr)) {
    return new NumberCommand();
  } 

  else  {
    input_stream->croak("Can't handle character: " + chr);
    throw;
  }
}
