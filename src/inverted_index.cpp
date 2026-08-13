#include "inverted_index.h"

void InvertedIndex::addDocument(int docId, const std::vector<std::string> &tokens)
{
  for (const std::string &token : tokens)
  {
    index[token][docId]++;
  }
  totalDocuments++;
}

int InvertedIndex::getTotalDocuments() const
{
  return totalDocuments;
}

std::unordered_map<int, int> InvertedIndex::search(const std::string &word) const
{
  auto it = index.find(word);
  if (it != index.end())
  {
    return it->second;
  }
  return {};
}
