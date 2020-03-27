#include <string>

#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

using namespace std;

class InputStream {
  public:
    char *input;
    InputStream();
    
    InputStream(char *str_stream);

    char next();

    char peek();

    bool eof();

    void croak(string msg, int line, int column);

    int get_line();
    int get_column();

  private:
    int pos = 0;
    int column = 0;
    int line = 1;
};

#endif