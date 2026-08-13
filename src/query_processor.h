#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include <unordered_map>
#include <vector>

class QueryProcessor
{
public:
  std::unordered_map<int, int> processAndQuery(
      const std::vector<std::unordered_map<int, int>> &wordMaps);

  std::unordered_map<int, int> processOrQuery(
      const std::vector<std::unordered_map<int, int>> &wordMaps);
};

#endif
