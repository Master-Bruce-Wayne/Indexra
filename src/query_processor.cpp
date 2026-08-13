#include "query_processor.h"

std::unordered_map<int, int> QueryProcessor::processAndQuery(
    const std::vector<std::unordered_map<int, int>> &wordMaps)
{
  std::unordered_map<int, int> result;

  if (wordMaps.empty())
    return result;

  result = wordMaps[0];

  for (size_t i = 1; i < wordMaps.size(); ++i)
  {
    std::unordered_map<int, int> temp;

    for (const auto &[docId, freq] : result)
    {
      auto it = wordMaps[i].find(docId);
      if (it != wordMaps[i].end())
      {
        temp[docId] = freq + it->second;
      }
    }

    result = std::move(temp);

    if (result.empty())
      break;
  }

  return result;
}

std::unordered_map<int, int> QueryProcessor::processOrQuery(
    const std::vector<std::unordered_map<int, int>> &wordMaps)
{
  std::unordered_map<int, int> result;

  for (const auto &wordMap : wordMaps)
  {
    for (const auto &[docId, freq] : wordMap)
    {
      result[docId] += freq;
    }
  }

  return result;
}
