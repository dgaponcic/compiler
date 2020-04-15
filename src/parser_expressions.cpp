#include "../include/parser_expressions.h"

using namespace std;

Number::Number(float value) {
  nb_value = value;
}
expression_type Number::get_type() {
  return number_expr;
}

String::String(string str) {
  value = str;
}
expression_type String::get_type() {
  return string_expr;
}

Boolean::Boolean(bool value) {
  bool_value = value;
}
expression_type Boolean::get_type() {
  return boolean_expr;
}

Variable::Variable(string ident) {
  identififier = ident;
}
expression_type Variable::get_type() {
  return var_expr;
}

Prog_Expression::Prog_Expression(vector<Expression *> exprs) {
  prog_exprs = exprs;
}
expression_type Prog_Expression::get_type() {
  return prog_expr;
}

Lambda::Lambda(vector<Expression *> params, Prog_Expression *lambda_body) {
  parameters = params;
  body = lambda_body;
}

expression_type Lambda::get_type() {
  return lambda_expr;
}

Call::Call(Variable *function_name, vector<Expression *> funct_args) {
  function = function_name;
  args = funct_args;
}
expression_type Call::get_type() {
  return call_expr;
}

Instant_Function_Call::Instant_Function_Call(Lambda *funct_body, vector<Expression *> funct_args) {
  body = funct_body;
  args = funct_args;
};

expression_type Instant_Function_Call::get_type() {
  return instant_f_call_expr;
}

If_Expression::If_Expression(Expression *condition, Prog_Expression *then_expr, Prog_Expression *else_expr) {
  cond = condition;
  then = then_expr;
  else_expression = else_expr;
}

expression_type If_Expression::get_type() {
  return if_expr;
}

Assigment::Assigment(string oper, Expression *left_branch, Expression *right_branch) {
  op = oper;
  right = right_branch;
  left = left_branch;
}
expression_type Assigment::get_type() {
  return assigment_expr;
}

Binary::Binary(string oper, Expression *left_branch, Expression *right_branch) {
  op = oper;
  right = right_branch;
  left = left_branch;
}

expression_type Binary::get_type() {
  return binary_expr;
}

Paren_Expr::Paren_Expr(Expression *expression) {
  expr = expression;
}

expression_type Paren_Expr::get_type() {
  return paren_expr;
}

Return_Expr::Return_Expr(Expression *return_expression) {
  ret_expr = return_expression;
}

expression_type Return_Expr::get_type() {
  return return_expr;
}
