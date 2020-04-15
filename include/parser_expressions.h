#include <string>
#include <vector>
#include <map>
#include "command.h"
#include "token_stream.h"

using namespace std; 

#ifndef PARSER_EXPRESSIONS_H
#define PARSER_EXPRESSIONS_H

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
  paren_expr = 12,
  instant_f_call_expr = 13,
};

class Expression {
  public:
    virtual expression_type get_type() = 0;
};

class Number: public Expression {
  public:

    float nb_value;
    Number(float value);

    expression_type get_type();
};

class String: public Expression {
  public:
    string value;
    String(string str);

    expression_type get_type();
};

class Boolean: public Expression {
  public:
    bool bool_value;
    Boolean(bool value);

    expression_type get_type();
};

class Variable: public Expression {
  public:
    string identififier;
    Variable(string ident);

    expression_type get_type();
};


class Prog_Expression: public Expression {
  public:
    vector<Expression*>prog_exprs;
    Prog_Expression(vector<Expression*>exprs);

    expression_type get_type();
};

class Lambda: public Expression {
  public:
    vector<Expression*> parameters;
    Prog_Expression *body;
    Lambda(vector<Expression*> params, Prog_Expression *lambda_body);

    expression_type get_type();
};

class Call: public Expression {
  public:
    Variable *function;
    vector<Expression*> args;
    Call(Variable *function_name, vector<Expression*> funct_args);

    expression_type get_type();
};

class Instant_Function_Call: public Expression {
  public:
    Lambda *body;
    vector<Expression*> args;
    Instant_Function_Call(Lambda *funct_body, vector<Expression*> funct_args);
    expression_type get_type();
};

class If_Expression: public Expression {
  public:
    Expression *cond;
    Prog_Expression *then;
    Expression *else_expression;

    If_Expression(Expression *condition, Prog_Expression *then_expr, Prog_Expression *else_expr = NULL);

    expression_type get_type();
};

class Assigment: public Expression {
  public:
    string op;
    Expression *right;
    Expression *left;
    Assigment(string oper, Expression *left_branch, Expression *right_branch);

    expression_type get_type();
};

class Binary: public Expression {
  public:
    string op;
    Expression *right;
    Expression *left;
    Binary(string oper, Expression *left_branch, Expression *right_branch);

    expression_type get_type();
};

class Paren_Expr: public Expression {
  public:
    Expression *expr;
    Paren_Expr(Expression *expression);

    expression_type get_type();
};


class Return_Expr: public Expression {
  public:
    Expression *ret_expr;
    Return_Expr(Expression *return_expression);

    expression_type get_type();
};

#endif
