#include "trie.h"

Trie::Trie()
{
  root = new TrieNode();
}

void Trie::insert(const std::string &word)
{
  TrieNode *node = root;

  for (char c : word)
  {
    if (node->children.find(c) == node->children.end())
    {
      node->children[c] = new TrieNode();
    }
    node = node->children[c];
  }

  node->isEndOfWord = true;
}

void Trie::dfs(TrieNode *node,
               std::string &current,
               std::vector<std::string> &results)
{
  if (node->isEndOfWord)
  {
    results.push_back(current);
  }

  for (const auto &[c, next] : node->children)
  {
    current.push_back(c);
    dfs(next, current, results);
    current.pop_back();
  }
}

std::vector<std::string> Trie::autocomplete(const std::string &prefix)
{
  TrieNode *node = root;

  for (char c : prefix)
  {
    if (node->children.find(c) == node->children.end())
    {
      return {};
    }
    node = node->children[c];
  }

  std::vector<std::string> results;
  std::string current = prefix;

  dfs(node, current, results);
  return results;
}
