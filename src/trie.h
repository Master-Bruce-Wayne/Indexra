#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>
#include <vector>

class TrieNode
{
public:
  bool isEndOfWord;
  std::unordered_map<char, TrieNode *> children;

  TrieNode() : isEndOfWord(false) {}
};

class Trie
{
public:
  Trie();
  void insert(const std::string &word);
  std::vector<std::string> autocomplete(const std::string &prefix);

private:
  TrieNode *root;

  void dfs(TrieNode *node,
           std::string &current,
           std::vector<std::string> &results);
};

#endif
