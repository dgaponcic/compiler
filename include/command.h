#include <string>
#include "input_stream.h"

using namespace std; 

#ifndef COMMAND_H
#define COMMAND_H

enum token_types {
  null_token = 0,
  var = 1,
  number = 2,
  op = 3,
  str = 4,
  punct = 5,
  keyword = 6,
  boolean = 7,
};

struct token {
  token_types type;
  string val;
};

class TokenCommand {
  public:
    virtual token execute(InputStream* input_stream) = 0;
    token curr_token;
};

class EOFCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class OperatorCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class PunctuationCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class StringCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class AlphaCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class NumberCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class BooleanCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class NullTokenCommand: virtual public TokenCommand {
  public:
    token execute(InputStream* input_stream);
};

class TokenCommandDispatcher {
  public:
    TokenCommand* dispatch(char chr, InputStream* input_stream);
};

#endif
