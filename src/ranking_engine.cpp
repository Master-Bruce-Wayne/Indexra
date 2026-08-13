#include "ranking_engine.h"
#include <queue>
#include <cmath>
#include <algorithm>

std::vector<std::pair<int, int>> RankingEngine::rankTF(
    const std::unordered_map<int, int> &docFreq,
    int k)
{
  std::priority_queue<
      std::pair<int, int>,
      std::vector<std::pair<int, int>>,
      std::greater<>>
      minHeap;

  for (const auto &[docId, tf] : docFreq)
  {
    minHeap.push({tf, docId});
    if (minHeap.size() > static_cast<size_t>(k))
      minHeap.pop();
  }

  std::vector<std::pair<int, int>> result;
  while (!minHeap.empty())
  {
    result.push_back({minHeap.top().second,
                      minHeap.top().first});
    minHeap.pop();
  }

  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<std::pair<int, double>> RankingEngine::rankTFIDF(
    const std::unordered_map<int, int> &docFreq,
    int totalDocs,
    int docsWithWord,
    int k)
{
  double idf = std::log(
      1.0 + static_cast<double>(totalDocs) / docsWithWord);

  std::priority_queue<
      std::pair<double, int>,
      std::vector<std::pair<double, int>>,
      std::greater<>>
      minHeap;

  for (const auto &[docId, rawTF] : docFreq)
  {
    double tf = 1.0 + std::log(rawTF);

    double score = tf * idf;
    minHeap.push({score, docId});

    if (minHeap.size() > static_cast<size_t>(k))
      minHeap.pop();
  }

  std::vector<std::pair<int, double>> result;
  while (!minHeap.empty())
  {
    result.push_back({minHeap.top().second,
                      minHeap.top().first});
    minHeap.pop();
  }

  std::reverse(result.begin(), result.end());
  return result;
}
