#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

class Tokenizer
{
private:
  bool isValidChar(char c);

public:
  std::vector<std::string> tokenize(const std::string &text);
};

#endif
