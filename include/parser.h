#include <string>
#include <vector>
#include <map>
#include "command.h"
#include "token_stream.h"

using namespace std; 

#ifndef PARSER_H
#define PARSER_H

enum expression_type {
  number_expr = 0,
  string_expr = 1,
  boolean_expr = 2,
  var_expr = 3,
  lambda_expr = 4,
  call_expr = 5,
  if_expr = 6,
  assigment_expr = 7,
  binary_expr = 8,
  null_expr = 9,
  prog_expr = 10,
  return_expr = 11,
};

class Expression {
  public:
    virtual expression_type get_type() = 0;
};

class Number: public Expression {
  public:

    float nb_value;
    public:
    Number ();
    Number(float value);

    expression_type get_type();

  private:
    expression_type __type = number_expr;
};

class String: public Expression {
  public:
    string value;

    String(string str);

    expression_type get_type();

  private:
    expression_type __type = string_expr;
};

class Boolean: public Expression {
  public:
    bool bool_value;
    Boolean();
    Boolean(bool value);

    expression_type get_type();

  private:
    expression_type __type = boolean_expr;
};

class Variable: public Expression {
  public:

    string identififier;
    Variable ();
    Variable(string ident);

    expression_type get_type();

  private:
    expression_type __type = var_expr;
};


class Prog_Expression: public Expression {
  public:

    vector<Expression*>prog_exprs;

    Prog_Expression(vector<Expression*>exprs);

    expression_type get_type();

  private:
    expression_type __type = prog_expr;
};

class Lambda: public Expression {
  public:

  vector<Expression*> parameters;
  Prog_Expression *body;
  Lambda(vector<Expression*> params, Prog_Expression *lambda_body);

  expression_type get_type();

  private:
    expression_type __type = lambda_expr;
};

class Call: public Expression {
  public:

    Variable *function;
    vector<Expression*> args;

    Call(Variable *function_name, vector<Expression*> funct_args);

    expression_type get_type();

  private:
    expression_type __type = call_expr;
};

class If_Expression: public Expression {
  public:

    Expression *cond;
    Prog_Expression *then;
    Expression *else_expression;

    If_Expression(Expression *condition, Prog_Expression *then_expr, Prog_Expression *else_expr = NULL);

    expression_type get_type();

  private:
    expression_type __type = if_expr;
};

class Assigment: public Expression {
  public:

    string op;
    Expression *right;
    Expression *left;

    Assigment(string oper, Expression *left_branch, Expression *right_branch);

    expression_type get_type();

  private:
    expression_type __type = assigment_expr;
};

class Binary: public Expression {
  public:

    string op;
    Expression *right;
    Expression *left;

    Binary(string oper, Expression *left_branch, Expression *right_branch);

    expression_type get_type();
  
  private:
    expression_type __type = binary_expr;
};

class Return_Expr: public Expression {
  public:
    Expression *ret_expr;

    Return_Expr(Expression *return_expression);

    expression_type get_type();

  private:
    expression_type __type = return_expr;
};

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

  Expression *parse_bool();

  bool is_punct(string ch);

  bool is_kw(string kw);
    
  bool is_op(string op);

  void skip_punct(string ch);

  void skip_kw(string kw);
  
  void skip_op(string op);

  Expression *maybe_call(function<Expression*()> expr);

  vector<Expression*> delimited(string start, string stop, string separator, function<Expression*()> parser);

  Prog_Expression *parse_prog();

  If_Expression *parse_if();

  Expression *parse_varname();

  Lambda *parse_lambda();

  Call *parse_call(Variable *funct);

  Return_Expr *parse_return();

  Expression *parse_atom();

  Expression *maybe_binary(Expression *left, int my_prec);

  Expression *parse_expression();

  vector<Expression*> parseTopLevel();

  vector<Expression*> parse();
};

void print_ast(Expression *expr, int ident = 0);

#endif
