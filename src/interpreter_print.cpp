#include "../include/interpreter_print.h"

void print_ast(Expression *expr, int ident) {
  switch (expr->get_type()) {
    case number_expr: {
      Number* number = (Number*)expr;
      cout << std::string(ident, ' ' ) << number->nb_value << '\n';
      break;
    }

    case string_expr: {
      String* str = (String*)expr;
      cout << std::string(ident, ' ' ) << str->value << '\n';
      break;
    }

    case boolean_expr: {
      Boolean* boolean = (Boolean*)expr;
      cout << std::string(ident, ' ' ) << boolean->bool_value << '\n';
      break;
    }

    case assigment_expr: {
      Assigment* assigment = (Assigment*)expr;
      cout << std::string(ident, ' ' ) << assigment->op << '\n';
      print_ast(assigment->left, ident + 2);
      print_ast(assigment->right, ident + 2);
      break;
    }

    case prog_expr: {
      Prog_Expression* prog_expression = (Prog_Expression*)expr;
      for (Expression *expression: prog_expression->prog_exprs) {
        print_ast(expression, ident + 2);
      }
      break;
    }

    case if_expr: {
      If_Expression* if_expression = (If_Expression*)expr;
      cout << std::string(ident, ' ' ) << "if" << '\n';
      print_ast(if_expression ->cond, ident + 2);
      cout << std::string(ident, ' ' ) << "then" << '\n';
      print_ast(if_expression ->then, ident + 2);
      if(if_expression->else_expression) {
        cout << std::string(ident, ' ' ) << "else" << '\n';
        print_ast(if_expression->else_expression, ident + 2);
      }

      break;
    }

    case paren_expr: {
      Paren_Expr* paren_expr = (Paren_Expr*)expr;
      cout << std::string(ident, ' ' ) << "paren" << '\n';
      print_ast(paren_expr->expr, ident + 2);
      break;
    }

    case binary_expr: {
      Binary* binary = (Binary*)expr;
      cout << std::string(ident, ' ' ) << binary->op << '\n';
      print_ast(binary->left, ident + 2);
      print_ast(binary->right, ident + 2);
      break;
    }

    case var_expr: {
      Variable* var = (Variable*)expr;
      cout << std::string(ident, ' ' ) << var->identififier << '\n';
      break;
    }

    case lambda_expr: {
      Lambda* lambda = (Lambda*)expr;
      cout << std::string(ident, ' ' ) << "lambda params" << '\n';
      for (Expression *param: lambda->parameters) {
        print_ast(param, ident + 2);
      }
      cout << std::string(ident, ' ' ) << "lambda body" << '\n';
      print_ast(lambda->body, ident + 2);
      break;
    }

    case call_expr: {
      Call* call = (Call*)expr;
      cout << std::string(ident, ' ' ) << "call" << '\n';
      print_ast(call->function, ident + 2);

      cout << std::string(ident, ' ' ) << "args" << '\n';
      for (Expression *arg: call->args) {
        print_ast(arg, ident + 2);
      }
      break;
    }

    case return_expr: {
      Return_Expr* return_expr = (Return_Expr*)expr;
      cout << std::string(ident, ' ' ) << "return" << '\n';
      print_ast(return_expr->ret_expr, ident + 2);
      break;
    }

    default:
      break;
  }
}