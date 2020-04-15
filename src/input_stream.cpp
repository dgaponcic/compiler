#include "../include/input_stream.h"
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

char InputStream::next() {
  char curr_char = input[pos];
  pos++;
  if (int(curr_char) == 10) {
    line++;
    column = 0;
  } else {
    column++;
  }
  return curr_char;
}

InputStream::InputStream() {}

InputStream::InputStream(char *str_stream) {
  input = str_stream;
}

char InputStream::peek() {
  return input[pos];
}

bool InputStream::eof() {
  return int(peek()) == 0;
}

int InputStream::get_line() {
  return line;
}

int InputStream::get_column() {
  return column;
}

void InputStream::croak(string msg, int line, int column) {
  throw std::runtime_error(string(msg) + " (" + to_string(line) + ":" + to_string(column) + ")");
}
