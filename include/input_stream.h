#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H


class InputStream {
  public:
    char *input;
    InputStream();
    
    InputStream(char *str_stream);

    char next();

    char peek();

    bool eof();

  private:
    int pos = 0;
    int column = 0;
    int line = 0;
};

#endif