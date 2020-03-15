#include <string>
#include <vector>
#include <bits/stdc++.h> 
#include "../include/utils.h"
#include "../include/input_stream.h"

#define MAX_VAR_MEMORY 1000

using namespace std;

char* read_while(function<bool(char)> predicate, InputStream* input_stream) {
  char *str = (char *)(malloc(sizeof(char) * MAX_VAR_MEMORY));
  int i = 0;

  while(!input_stream->eof() && predicate(input_stream->peek())) {
    str[i] = input_stream->next();
    i += 1;
  }
  str[i] = '\0';
  return str;
}

bool is_string_delimitator(char ch) {
  return ch == '"';
}

bool is_whitespace(char ch) {
  return ch == ' ' || ch == '\n';
}

bool contains(char ch, char array_elements[]) {
  bool res = false;
  int len = get_length(array_elements);
  for(int i = 0; i < len; i++){
    if(array_elements[i] == ch) {
      res = true;
      break;
    }
  }
  return res;
}

bool contains(const std::string &value, const vector<string> &array) {
  return find(array.begin(), array.end(), value) != array.end();
}

bool is_punctuation(char ch) {
  char punctuation[6] = {';', '(', ')', '{', '}', ','};
  return contains(ch, punctuation);
}

bool is_operator(char ch) {
  char operators[7] = {'=', '>', '<', '-', '+', '/', '*'};
  return contains(ch, operators);
}

bool is_bool_operator(string str) {
  vector<string> keywords = {"and", "not", "or"};
  return contains(str, keywords);
}

bool is_boolean(string str) {
  vector<string> keywords = {"true", "false"};
  return contains(str, keywords);
}

bool is_string_char(char ch) {
  char symbols[] = {' ', ',', '.', ':'}; 
  return isalnum(ch) | contains(ch, symbols);
}

bool is_alpha_num_symb(char ch) {
  char symbols[5] = {'!', '*', '?', '_'};
  return isalnum(ch) | contains(ch, symbols);
}

int get_length(char arr[]) {
  int i = 0;
  while (arr[i]) {
    i += 1;
  }
  return i;
}

bool is_keyword(string str) {
  vector<string> keywords = {"lambda", "if", "return", "else"};
  return contains(str, keywords);
}

bool is_digit(char ch) {
  return isdigit(ch);
}

char *read_program(char program_name[]) {
  FILE *f = fopen(program_name, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *str = (char *)malloc(fsize + 1);
  fread(str, fsize, 1, f);
  fclose(f);

  str[fsize] = '\0';
  return str;
}
