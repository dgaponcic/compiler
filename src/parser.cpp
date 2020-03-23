#include "../include/parser.h"
#include "../include/command.h"
#include "../include/token_stream.h"

using namespace std;

Number::Number() {};
Number::Number(float value) {
  nb_value = value;
}
expression_type Number::get_type() {
  return __type;
}


String::String(string str) {
  value = str;
}
expression_type String::get_type() {
return __type;
}


Boolean::Boolean() {}
Boolean::Boolean(bool value) {
  bool_value = value;
}
expression_type Boolean::get_type() {
  return __type;
}


Variable::Variable () {}
Variable::Variable(string ident) {
  identififier = ident;
}
expression_type Variable::get_type() {
  return __type;
}


Prog_Expression::Prog_Expression(vector<Expression*>exprs) {
  prog_exprs = exprs;
}
expression_type Prog_Expression::get_type() {
  return __type;
}


Lambda::Lambda(vector<Expression*> params, Prog_Expression *lambda_body) {
  parameters = params;
  body = lambda_body;
}

expression_type Lambda::get_type() {
  return __type;
}


Call::Call(Variable *function_name, vector<Expression*> funct_args) {
  function = function_name;
  args = funct_args;
}
expression_type Call::get_type() {
  return __type;
}


If_Expression::If_Expression(Expression *condition, Prog_Expression *then_expr, Prog_Expression *else_expr) {
  cond = condition;
  then = then_expr;
  else_expression = else_expr;
}

expression_type If_Expression::get_type() {
  return __type;
}


Assigment::Assigment(string oper, Expression *left_branch, Expression *right_branch) {
  op = oper;
  right = right_branch;
  left = left_branch;
}
expression_type Assigment::get_type() {
  return __type;
}


Binary::Binary(string oper, Expression *left_branch, Expression *right_branch) {
  op = oper;
  right = right_branch;
  left = left_branch;
}

expression_type Binary::get_type() {
  return __type;
}


Return_Expr::Return_Expr(Expression *return_expression) {
  ret_expr = return_expression;
}

expression_type Return_Expr::get_type() {
  return __type;
}


  Parser::Parser(TokenStream *stream) {
    token_stream = stream;
  }

  Expression *Parser::parse_bool() {
    bool bool_val = token_stream->next().val == "true";
    Expression *bool_res = new Boolean(bool_val);
    return bool_res;
  }

  bool Parser::is_punct(string ch) {
    token tok = token_stream->peek();
    return tok.type == 5 && tok.val == ch;
  }

  bool Parser::is_kw(string kw) {
    token tok = token_stream->peek();
    return tok.type == 6 && tok.val == kw;
  }
    
  bool Parser::is_op(string op) {
    token tok = token_stream->peek();
    return tok.type == 3 && tok.val == op;
  }

  void Parser::skip_punct(string ch) {
        if (is_punct(ch)) {
          token_stream->next();
        }
  }

  void Parser::skip_kw(string kw) {
        if (is_kw(kw)) {
          token_stream->next();
        }
    }
  
  void Parser::skip_op(string op) {
    if (is_op(op)) {
      token_stream->next();
    }
  }

  Expression *Parser::maybe_call(function<Expression*()> expr) {
    Expression *expression = expr();
    if (is_punct("(")) {
      return parse_call((Variable*)expression);
    } else {
      return expression;
    }
  }

vector<Expression*> Parser::delimited(string start, string stop, string separator, function<Expression*()> parser) {
  vector<Expression*> prog;
  bool first = true;
  skip_punct(start);
  while(!token_stream->eof()) {
    if (is_punct(stop)) {
      break;
    }

    if (first) {
      first = false;
    } else {
      skip_punct(separator);
    }

    if (is_punct(stop)) {
      break;
    }
    prog.push_back(parser());
  }
  skip_punct(stop);
  return prog;
}

Prog_Expression *Parser::parse_prog() {
  Prog_Expression *prog = new Prog_Expression(delimited("{", "}", ";", [&]() {return parse_expression();} ));
  return prog;
}

If_Expression *Parser::parse_if() {
  skip_kw("if");
  Expression *cond = parse_expression();
  Expression *then = parse_expression();
  if (is_kw("else")) {
    skip_kw("else");
    Expression *else_expr = parse_expression();
    return new If_Expression(cond, (Prog_Expression*)then, (Prog_Expression*)else_expr);
  }
  return new If_Expression(cond, (Prog_Expression*)then);
}

Expression *Parser::parse_varname() {
  token curr = token_stream->peek();
  token_stream -> next();
  return (Expression*)new Variable(curr.val);
}

Lambda *Parser::parse_lambda() {
  skip_kw("lambda");
  vector<Expression*> parameters = delimited("(", ")", ",", [&](){ return parse_varname(); });
  Prog_Expression *body = (Prog_Expression*)parse_expression();
  return new Lambda(parameters, body);
}

Call *Parser::parse_call(Variable *funct) {
  vector<Expression*> args = delimited("(", ")", ",", [&](){ return parse_expression(); });
  return new Call(funct, args);
}

Return_Expr *Parser::parse_return() {
  skip_kw("return");
  Expression *return_expression = parse_expression();
  return new Return_Expr(return_expression);
}

Expression *Parser::parse_atom() {

    return maybe_call([&](){

      if (is_punct("(")) {
        token_stream->next();
        Expression *expr = parse_expression();
        skip_punct("(");
        return expr;
      }

      if (is_punct("{")) {
        return (Expression*)parse_prog();
      }
      if (is_kw("if")) {
        return (Expression*)parse_if();
      }

      if (token_stream->peek().type == 7) {
        return parse_bool();
      }

      if (is_kw("lambda")) {
        return (Expression*)parse_lambda();
      }

      if (is_kw("return")) {
        return (Expression*)parse_return();
      }

      token tok = token_stream->next();

      if(tok.type == 2) {
        Expression *res = new Number(strtof(tok.val.c_str(), 0));
        return res;
      }

      if(tok.type == 4) {
        Expression *res = new String(tok.val);
        return res;
      }

      if(tok.type == 1) {
        Expression *res = new Variable(tok.val);;
        return res;
      }
    });
  }

  Expression *Parser::maybe_binary(Expression *left, int my_prec) {
    token tok = token_stream->peek();

    if (tok.type == 3) {
      int his_prec;
      his_prec = op_precedence[tok.val];
      if (his_prec > my_prec) {
        token_stream->next();
        Expression *left_expr;
        if(tok.val == "=") {
          left_expr = new Assigment(tok.val, left, maybe_binary(parse_atom(), his_prec));
        } else {
          left_expr = new Binary(tok.val, left, maybe_binary(parse_atom(), his_prec));
        }
        return maybe_binary(left_expr, my_prec);
      }
    }
    return left;
  }

  Expression *Parser::parse_expression() {
    return maybe_call([&](){
      return maybe_binary(parse_atom(), 0);
    });
  }

  vector<Expression*> Parser::parseTopLevel() {
    vector<Expression*> ast;
    token_stream->next();
    while(!token_stream ->eof() && token_stream->peek().type != null_token) {
      ast.push_back(parse_expression());
      if (!token_stream->eof() && token_stream->peek().type == 5 && token_stream->peek().val == ";") {

        token_stream->next();
      }
    }
    return ast;
  }

  vector<Expression*> Parser::parse() {
    return parseTopLevel();
  }


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