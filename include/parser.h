#include <string>
#include <vector>
#include <map>
#include "command.h"
#include "token_stream.h"
#include "parser_expressions.h"

using namespace std; 

#ifndef PARSER_H
#define PARSER_H

class Parser {
  public:

  map<string, int> op_precedence = {
    {"=", 1},
    {"or", 2},
    {"and", 3},
    {"<", 7}, {">", 7}, {"<=", 7}, {">=", 7}, {"==", 7}, {"!=", 7},
    {"+", 10}, {"-", 10},
    {"*", 20}, {"/", 20}, {"%", 20},
  };

  TokenStream *token_stream;
  Parser(TokenStream *stream);

  Expression *parse_bool(string val);

  bool is_punct(string ch);
  bool is_kw(string kw);
  bool is_op(string op);

  void skip_punct(string ch);
  void skip_kw(string kw);
  void skip_op(string op);

  void unexpected();

  Prog_Expression *parse_prog();
  If_Expression *parse_if();
  Expression *parse_varname();
  Lambda *parse_lambda();
  Call *parse_call(Variable *funct);
  Return_Expr *parse_return();
  Number *parse_negative_numbers();

  Expression *parse_keyword();
  Expression *parse_punct();
  Expression *parse_simple_types(token tok);
  Expression *parse_atom();
  Expression* read_paren_expr();
  Expression *maybe_call(function<Expression*()> expr);
  Expression *maybe_binary(Expression *left, int my_prec);
  Expression *parse_expression(bool is_function_open = false);

  vector<Expression*> delimited(string start, string stop, string separator, function<Expression*()> parser);
  vector<Expression*> parse();
};


#endif
