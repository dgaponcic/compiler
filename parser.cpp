#include <stdio.h>
#include <fstream>
#include "include/token_stream.h"
#include "include/utils.h"
#include <string>
#include <vector>
#include <map>

using namespace std; 

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

map<string, int> precedence = {
  {"=", 1},
  {"or", 2},
  {"and", 3},
  {"<", 7}, {">", 7}, {"<=", 7}, {">=", 7}, {"==", 7}, {"!=", 7},
  {"+", 10}, {"-", 10},
  {"*", 20}, {"/", 20}, {"%", 20},
};

class Expression {
  public:
    virtual expression_type get_type() = 0;
};

class Number: public Expression {
  public:

    float nb_value;
    public:
    Number () {}
    Number(float value) {
      nb_value = value;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = number_expr;
};

class String: public Expression {
  public:
    string value;

    String(string str) {
      value = str;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = string_expr;
};

class Boolean: public Expression {
  public:
    bool bool_value;
    Boolean() {}
    Boolean(bool value) {
      bool_value = value;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = boolean_expr;
};

class Variable: public Expression {
  public:

    string identififier;
    Variable () {}
    Variable(string ident) {
      identififier = ident;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = var_expr;
};


class Prog_Expression: public Expression {
  public:

    vector<Expression*>prog_exprs;

    Prog_Expression(vector<Expression*>exprs) {
      prog_exprs = exprs;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = prog_expr;
};

class Lambda: public Expression {
  public:

  vector<Expression*> parameters;
  Prog_Expression *body;
  Lambda(vector<Expression*> params, Prog_Expression *lambda_body) {
    parameters = params;
    body = lambda_body;
  }

  expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = lambda_expr;
};

class Call: public Expression {
  public:

    Variable *function;
    vector<Expression*> args;

    Call(Variable *function_name, vector<Expression*> funct_args) {
      function = function_name;
      args = funct_args;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = call_expr;
};

class If_Expression: public Expression {
  public:

    Expression *cond;
    Prog_Expression *then;
    Expression *else_expression;

    If_Expression(Expression *condition, Prog_Expression *then_expr, Prog_Expression *else_expr = NULL) {
      cond = condition;
      then = then_expr;
      else_expression = else_expr;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = if_expr;
};

class Assigment: public Expression {
  public:

    string op;
    Expression *right;
    Expression *left;

    Assigment(string oper, Expression *left_branch, Expression *right_branch) {
      op = oper;
      right = right_branch;
      left = left_branch;
    }

    expression_type get_type() {
        return __type;
      }

  private:
    expression_type __type = assigment_expr;
};

class Binary: public Expression {
  public:

    string op;
    Expression *right;
    Expression *left;

    Binary(string oper, Expression *left_branch, Expression *right_branch) {
      op = oper;
      right = right_branch;
      left = left_branch;
    }

    expression_type get_type() {
        return __type;
      }
  
  private:
    expression_type __type = binary_expr;
};

class Return_Expr: public Expression {
  public:
    Expression *ret_expr;

    Return_Expr(Expression *return_expression) {
      ret_expr = return_expression;
    }

    expression_type get_type() {
      return __type;
    }

  private:
    expression_type __type = return_expr;
};



class Parser {
  public:
  TokenStream *token_stream;
  Parser(TokenStream *stream) {
    token_stream = stream;
  }

  Expression *parse_bool() {
    bool bool_val = token_stream->next().val == "true";
    Expression *bool_res = new Boolean(bool_val);
    return bool_res;
  }

  bool is_punct(string ch) {
    token tok = token_stream->peek();
    return tok.type == 5 && tok.val == ch;
  }

  bool is_kw(string kw) {
    token tok = token_stream->peek();
    return tok.type == 6 && tok.val == kw;
  }
    
  bool is_op(string op) {
    token tok = token_stream->peek();
    return tok.type == 3 && tok.val == op;
  }

  void skip_punct(string ch) {
        if (is_punct(ch)) {
          token_stream->next();
        }
  }

  void skip_kw(string kw) {
        if (is_kw(kw)) {
          token_stream->next();
        }
    }
  
  void skip_op(string op) {
    if (is_op(op)) {
      token_stream->next();
    }
  }

  Expression *maybe_call(function<Expression*()> expr) {
    Expression *expression = expr();
    if (is_punct("(")) {
      return parse_call((Variable*)expression);
    } else {
      return expression;
    }
  }

vector<Expression*> delimited(string start, string stop, string separator, function<Expression*()> parser) {
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

Prog_Expression *parse_prog() {
  Prog_Expression *prog = new Prog_Expression(delimited("{", "}", ";", [&]() {return parse_expression();} ));
  return prog;
}

If_Expression *parse_if() {
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

Expression *parse_varname() {
  token curr = token_stream->peek();
  token_stream -> next();
  return (Expression*)new Variable(curr.val);
}

Lambda *parse_lambda() {
  skip_kw("lambda");
  vector<Expression*> parameters = delimited("(", ")", ",", [&](){ return parse_varname(); });
  Prog_Expression *body = (Prog_Expression*)parse_expression();
  return new Lambda(parameters, body);
}

Call *parse_call(Variable *funct) {
  vector<Expression*> args = delimited("(", ")", ",", [&](){ return parse_expression(); });
  return new Call(funct, args);
}

Return_Expr *parse_return() {
  skip_kw("return");
  Expression *return_expression = parse_expression();
  return new Return_Expr(return_expression);
}

Expression *parse_atom() {

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

  Expression *maybe_binary(Expression *left, int my_prec) {
    token tok = token_stream->peek();

    if (tok.type == 3) {
      int his_prec;
      his_prec = precedence[tok.val];
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

  Expression *parse_expression() {
    return maybe_call([&](){
      return maybe_binary(parse_atom(), 0);
    });
  }

  vector<Expression*> parseTopLevel() {
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

  vector<Expression*> parse() {
    return parseTopLevel();
  }
};

void print_tokens(token curr_token) {
  cout << curr_token.type << ' ' << curr_token.val << '\n';
}

void print_ast(Expression *expr, int ident = 0) {
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

int main(int argc, char **argv) {
  if (argc < 2) {
    exit(1);
  }

  char* program_name = argv[1];
  char* my_program = read_program(program_name);
  TokenStream token_stream = TokenStream(my_program);

  // while (!token_stream.eof()) {
  //   print_tokens(token_stream.peek());
  //   token_stream.next();
  // }

  Parser parser = Parser(&token_stream);
  vector<Expression*> res = parser.parse();
  for (int i = 0; i < res.size(); i += 1) {
    print_ast(res[i]);
  }

  return 0;
}