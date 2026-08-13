#ifndef RANKING_ENGINE_H
#define RANKING_ENGINE_H

#include <unordered_map>
#include <vector>
#include <utility>

class RankingEngine
{
public:
  std::vector<std::pair<int, int>> rankTF(
      const std::unordered_map<int, int> &docFreq,
      int k);

  std::vector<std::pair<int, double>> rankTFIDF(
      const std::unordered_map<int, int> &docFreq,
      int totalDocs,
      int docsWithWord,
      int k);
};

#endif
