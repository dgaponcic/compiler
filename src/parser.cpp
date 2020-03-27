
#include "../include/parser.h"
#include "../include/command.h"
#include "../include/token_stream.h"

using namespace std;

Parser::Parser(TokenStream *stream) {
  token_stream = stream;
}

Expression *Parser::parse_bool(string val) {
  bool bool_val = val == "true";
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
  } else {
    // cout << "lll " << token_stream ->line << " " << token_stream->column << "\n";
    token_stream->croak("Expecting Punctuation: \\" + ch + "\\.", true);
    throw;
  }
}

void Parser::skip_kw(string kw) {
  if (is_kw(kw)) {
    token_stream->next();
  }  else {
      token_stream->croak("Expecting Keyword: \\" + kw + "\\.", true);
      throw;
    }
}
  
void Parser::skip_op(string op) {
  if (is_op(op)) {
    token_stream->next();
  } else {
        // cout << "lll " << token_stream ->line << " " << token_stream->column << "\n";
      token_stream->croak("Expecting Operator: \\" + op + "\\.", true);
      throw;
    }
}

void Parser::unexpected() {
  token_stream->croak("Unexpected token: " + token_stream->peek().val, false);
}

Expression *Parser::maybe_call(function<Expression*()> expr) {
  Expression *expression = expr();
    
  if (is_punct("(") && expression->get_type() == var_expr) {
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
    if (is_punct(stop)) break;

    if (first) {
      first = false;
    } else {
      skip_punct(separator);
    }

    if (is_punct(stop)) break;
    prog.push_back(parser());
  }

  skip_punct(stop);
  return prog;
}

Prog_Expression *Parser::parse_prog() {
  vector<Expression*> prog;
  bool first = true;
  skip_punct("{");
  while(!token_stream->eof()) {
    if (is_punct("}")) break;
    prog.push_back(parse_expression());
    // cout << "in prog " << 
    skip_punct(";");
  }

  skip_punct("}");
  return new Prog_Expression(prog);
}

If_Expression *Parser::parse_if() {
  skip_kw("if");
  Expression *cond = parse_expression();
  Expression *then = parse_prog();

  if (is_kw("else")) {
    skip_kw("else");
    Expression *else_expr = parse_prog();
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
  Prog_Expression *body = parse_prog();
  return new Lambda(parameters, body);
}

Call *Parser::parse_call(Variable *funct) {
  vector<Expression*> args = delimited("(", ")", ",", [&](){ return parse_expression(false); });
  return new Call(funct, args);
}

Return_Expr *Parser::parse_return() {
  skip_kw("return");
  Expression *return_expression = parse_expression();
  return new Return_Expr(return_expression);
}

Expression *Parser::parse_keyword() {
  if (is_kw("if")) {
    return (Expression*)parse_if();
  }
  else if (is_kw("lambda")) {
    return (Expression*)parse_lambda();
  }
  else if (is_kw("return")) {
    Expression *ret = (Expression*)parse_return();
    return ret;
  }
  unexpected();
  throw;
}

Expression *Parser::parse_punct() {
  if (is_punct("(")) {
    Expression *expr = new Paren_Expr(read_paren_expr());
    return expr;
  }
  else if (is_punct("{")) {
    return (Expression*)parse_prog();
  }
  unexpected();
  throw;
}

Number *Parser::parse_negative_numbers() {
  token tok = token_stream->peek();
  if (tok.type == 2) {
    Number *res = new Number(-strtof(tok.val.c_str(), 0));
      token_stream->next();
      return res;
  }
  unexpected();
  throw;
}

Expression *Parser::parse_simple_types(token tok) {

  switch (tok.type) {
    case 2: {
      return new Number(strtof(tok.val.c_str(), 0));
    }
    case 7: {
      return parse_bool(tok.val);
    }
    case 4: {
      return new String(tok.val);
    }
    case 1: {
      return new Variable(tok.val);;
    }
    case 3: {
      if (tok.val == "-") {
        token_stream->next();
        return parse_negative_numbers();
      }
      break;
    }

    default:
      break;
  }
  unexpected();
  throw;
}

Expression *Parser::parse_atom() {
  return maybe_call([&](){
    if (token_stream->peek().type == 5) {
      return parse_punct();
    }
    else if (token_stream->peek().type == 6) {
      return parse_keyword();
    }

    token tok = token_stream->next();
    return parse_simple_types(tok);
  });
}

Expression* Parser::read_paren_expr() {
  Expression* expr;
  skip_punct("(");
  expr = maybe_binary(parse_atom(), 0);
  skip_punct(")");
  return expr;
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

Expression *Parser::parse_expression(bool is_function_open) {
  return maybe_call([&](){
    return maybe_binary(parse_atom(), 0);
  });
}

vector<Expression*> Parser::parse() {
  vector<Expression*> ast;
  token_stream->next();
  while(!token_stream ->eof() && token_stream->peek().type != 0) {
    ast.push_back(parse_expression());
    skip_punct(";");
  }
  return ast;
}
