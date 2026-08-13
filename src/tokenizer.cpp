#include "tokenizer.h"
#include <cctype>

bool Tokenizer::isValidChar(char c)
{
  return std::isalnum(static_cast<unsigned char>(c));
}

std::vector<std::string> Tokenizer::tokenize(const std::string &text)
{
  std::vector<std::string> tokens;
  std::string current;

  for (char c : text)
  {
    if (isValidChar(c))
    {
      current += std::tolower(c);
    }
    else
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
    }
  }
  if (!current.empty())
  {
    tokens.push_back(current);
  }
  return tokens;
}
