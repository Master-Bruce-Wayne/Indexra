#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "inverted_index.h"
#include "query_processor.h"
#include "ranking_engine.h"
#include "tokenizer.h"
#include "trie.h"

// ANSI Color Codes for terminal aesthetics
namespace Color {
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string GREEN = "\033[32m";
const std::string CYAN = "\033[36m";
const std::string YELLOW = "\033[33m";
const std::string RED = "\033[31m";
const std::string MAGENTA = "\033[35m";
} // namespace Color

void printBanner() {
  std::cout << Color::CYAN << Color::BOLD << R"(
  ___           _
 |_ _|_ __   __| | _____  ___ __ __ _
  | || '_ \ / _` |/ _ \ \/ / '__/ _` |
  | || | | | (_| |  __/>  <| | | (_| |
 |___|_| |_|\__,_|\___/_/\_\_|  \__,_|

)" << Color::RESET;
  std::cout << Color::BOLD << " High-Performance Search Engine Core (C++23)\n"
            << Color::RESET;
  std::cout << " Type your query, or type " << Color::RED << "'/exit'"
            << Color::RESET << " to quit.\n";
  std::cout << " Type " << Color::MAGENTA << "'/auto <prefix>'" << Color::RESET
            << " for autocomplete demo.\n";
  std::cout << std::string(50, '=') << "\n\n";
}

int main() {
  Tokenizer tokenizer;
  InvertedIndex index;
  RankingEngine ranker;
  QueryProcessor processor;
  Trie trie;

  std::vector<std::string> docs = {
      "Data Structures and Algorithms form the core of computer science",
      "advanced data structures like Segment trees and Fenwick trees",
      "graph algorithms include bfs, dfs, Dijkstra and Bellman ford",
      "dynamic programming is a powerful ALGorithmic technique",
      "competitive programming requires strong data structures knowledge.",
      "C plus plus is widely used for systems programming and performance",
      "hash maps and hash TABles provide average constant time access.",
      "binary search trees heaps and tries are fundamental data structures",
      "search engines use inverted index ranking and query processing",
      "tf idf is a ranking ALGorithm used in information retrieval",
      "autocomplete systems use trie data structures for fast prefix search",
      "large scale systems require efficient indexing and caching.",
      "algorithms and data structures are essential for coding interviews",
      "machine learning models depend on data preprocessing and algorithms",
      "operating systems use SCHeduling algorithms and memory management"};

  std::cout << Color::YELLOW << "[SYSTEM] " << Color::RESET
            << "Booting Indexra and ingesting corpus...\n";

  auto start_index = std::chrono::high_resolution_clock::now();
  for (size_t docId = 0; docId < docs.size(); ++docId) {
    auto tokens = tokenizer.tokenize(docs[docId]);
    index.addDocument(static_cast<int>(docId), tokens);
    for (const auto &token : tokens)
      trie.insert(token);
  }
  auto end_index = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> index_time =
      end_index - start_index;

  std::cout << Color::GREEN << "[OK] " << Color::RESET << "Indexed "
            << index.getTotalDocuments() << " documents in " << Color::BOLD
            << index_time.count() << " ms\n\n"
            << Color::RESET;

  printBanner();

  std::string input;
  const int AND_BOOST = 10;

  while (true) {
    std::cout << Color::CYAN << "indexra> " << Color::RESET;
    if (!std::getline(std::cin, input) || input == "/exit")
      break;
    if (input.empty())
      continue;

    if (input.substr(0, 5) == "/auto") {
      std::string prefix = input.size() > 6 ? input.substr(6) : "";
      auto suggestions = trie.autocomplete(prefix);
      std::cout << Color::MAGENTA << " Autocomplete for '" << prefix << "':\n"
                << Color::RESET;
      if (suggestions.empty())
        std::cout << "   (No suggestions)\n";
      else
        for (const auto &word : suggestions)
          std::cout << "   - " << word << "\n";
      std::cout << "\n";
      continue;
    }

    auto start_query = std::chrono::high_resolution_clock::now();

    auto queryTokens = tokenizer.tokenize(input);
    if (queryTokens.empty())
      continue;

    std::vector<std::unordered_map<int, int>> postingLists;
    for (const auto &token : queryTokens) {
      postingLists.push_back(index.search(token));
    }

    auto andResult = processor.processAndQuery(postingLists);
    auto orResult = processor.processOrQuery(postingLists);

    std::unordered_map<int, int> blended = orResult;
    for (const auto &[docId, score] : andResult) {
      blended[docId] += score * AND_BOOST;
    }

    auto ranked =
        ranker.rankTFIDF(blended, index.getTotalDocuments(), blended.size(), 5);

    auto end_query = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> query_time =
        end_query - start_query;

    std::cout << "\n"
              << Color::GREEN << " [" << ranked.size() << " results found in "
              << query_time.count() << " µs]\n"
              << Color::RESET;

    std::cout << std::string(60, '-') << "\n";
    std::cout << std::setw(6) << "Rank" << std::setw(6) << "DocID"
              << std::setw(12) << "Score" << "  Snippet\n";
    std::cout << std::string(60, '-') << "\n";

    int r = 1;
    for (const auto &[docId, score] : ranked) {
      std::cout << std::setw(6) << r++ << std::setw(6) << docId << std::setw(12)
                << std::fixed << std::setprecision(4) << score << "  "
                << Color::BOLD << docs[docId].substr(0, 45) << "..."
                << Color::RESET << "\n";
    }
    std::cout << "\n";
  }

  std::cout << Color::YELLOW << "Shutting down Indexra.\n" << Color::RESET;
  return 0;
}
