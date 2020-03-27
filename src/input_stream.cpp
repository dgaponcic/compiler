#include "../include/input_stream.h"
#include <string>
#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

char InputStream::next() {
  char curr_char = input[pos]; 
  pos++;
  if (int(curr_char) == 10) {
    // cout << "line increased " << peek() << "\n";
    line++;
    column = 0;
  } else {
    // cout << "incresing column " << peek() << "\n";
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
