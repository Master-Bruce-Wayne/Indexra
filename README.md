# 🔍 Indexra

### A Search Engine Core Built from Scratch in C++

Indexra is a **from-scratch search engine core** implemented in **modern C++**, designed to demonstrate strong proficiency in **Data Structures, Algorithms, and System Design**.

The project replicates how real search engines work internally by implementing **tokenization, inverted indexing, TF-IDF ranking, AND/OR query processing, and trie-based autocomplete**, all exposed through a clean and explainable CLI.

---

## ✨ Features

### 🔹 Text Processing

- Tokenization and normalization (lowercasing, punctuation removal)
- Single pipeline used consistently for indexing and querying

### 🔹 Inverted Index

- Efficient mapping of
  `word → (document → frequency)`
- Constant-time average lookup using hash maps

### 🔹 Query Processing

- **AND queries** (high-precision, higher priority)
- **OR queries** (fallback for recall)
- Clean separation of query logic

### 🔹 Ranking Engine

- TF-IDF ranking with:
  - Log-normalized Term Frequency
  - Smoothed Inverse Document Frequency
- Heap-based Top-K ranking (`O(n log k)`)

### 🔹 Autocomplete

- Trie (prefix tree) based autocomplete
- Efficient prefix search
- Demonstrates tree-based DSA

### 🔹 CLI Demonstration

- Step-by-step output showing:
  - Indexing
  - Query parsing
  - AND/OR processing
  - Ranking
  - Autocomplete
- Easy to understand and explain in interviews

---

## 📁 Project Structure

```
src/
├── main.cpp # CLI and system integration
├── tokenizer.h/.cpp # Text tokenization and normalization
├── inverted_index.h/.cpp # Inverted index implementation
├── query_processor.h/.cpp # AND / OR query logic
├── ranking_engine.h/.cpp # TF and TF-IDF ranking
├── trie.h/.cpp # Autocomplete using Trie
```

## 🧪 Demonstrated Test Cases

Indexra is tested using a **realistic document corpus** containing algorithmic, systems, and information-retrieval topics.

### Example Queries

- **AND-dominant query:**
  `"data structures"`
- **OR-dominant query:**
  `"ranking algorithm"`

### Autocomplete Examples

- `dat` → data
- `alg` → algorithm, algorithms
- `str` → structures
- `aut` → autocomplete

These tests demonstrate:

- AND vs OR behavior
- TF-IDF ranking quality
- Boosting of full matches
- Trie-based prefix search

---

## ▶️ Build & Run

### Compile (from `src/` directory)

```bash
g++ -std=c++23 -O2 -Wall main.cpp tokenizer.cpp inverted_index.cpp \
ranking_engine.cpp query_processor.cpp trie.cpp -o indexra
```

### Run

```bash
./indexra
```
