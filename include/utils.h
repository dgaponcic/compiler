#include <stdlib.h>
#include <vector>
#include <bits/stdc++.h> 
#include "../include/input_stream.h"

using namespace std;

#ifndef UTILS_H
#define UTILS_H

int get_length(char arr[]);
char *read_program(char program_name[]);
char* read_while(function<bool(char)> predicate, InputStream* input_stream);
bool is_punctuation(char ch);
bool is_operator(char ch);
bool is_alpha_num_symb(char ch);
bool contains(char ch, char array_elements[]);
bool contains(const std::string &value, const vector<string> &array);
bool is_keyword(string str);
bool is_digit(char ch);
bool is_whitespace(char ch);
bool is_string_char(char ch);
bool is_string_delimitator(char ch);
bool is_boolean(string str);
bool is_bool_operator(string str);

#endif
