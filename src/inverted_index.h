#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <unordered_map>
#include <vector>

class InvertedIndex
{
public:
  void addDocument(int docId, const std::vector<std::string> &tokens);
  std::unordered_map<int, int> search(const std::string &word) const;

  int getTotalDocuments() const;

private:
  std::unordered_map<std::string, std::unordered_map<int, int>> index;
  int totalDocuments = 0;
};

#endif
