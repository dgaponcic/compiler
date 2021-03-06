#include "../include/input_stream.h"
#include "../include/command.h"
#include "../include/utils.h"

using namespace std; 

#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H


class TokenStream {
  public:
    InputStream input_stream;
    token curr_token;
    TokenCommandDispatcher tcd;

    TokenStream(char *input);
    TokenStream();

    token next();
    token peek();
    bool eof();
    void croak(string msg, bool prev_tok_pos);

    int line;
    int column;
};

#endif
