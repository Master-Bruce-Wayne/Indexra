# 🔍 Indexra

### High-Performance Search Engine Core Built from Scratch in C++23

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/Build-CMake%203.10%2B-064F8C.svg?style=flat&logo=cmake)](https://cmake.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Standard](https://img.shields.io/badge/Architecture-Modular-success.svg)](#-system-architecture)

**Indexra** is a high-performance, from-scratch search engine core written in modern **C++23**. It demonstrates the fundamental mechanics that power production search engines and information retrieval systems—including **lexical tokenization**, **inverted index construction**, **boolean query resolution (AND/OR)**, **hybrid TF-IDF relevance ranking**, and **trie-based autocomplete**—all accessible via an interactive, colored terminal interface.

---

## 📑 Table of Contents

- [System Architecture](#-system-architecture)
- [System Lifecycle & Data Flow](#-system-lifecycle--data-flow)
- [How It Works: Component Deep-Dive](#-how-it-works-component-deep-dive)
  - [1. Tokenization & Normalization](#1-tokenization--normalization)
  - [2. Inverted Index Construction](#2-inverted-index-construction)
  - [3. Boolean Query Processor](#3-boolean-query-processor)
  - [4. TF-IDF Ranking Engine & Top-K Selection](#4-tf-idf-ranking-engine--top-k-selection)
  - [5. Trie Autocomplete](#5-trie-autocomplete)
- [Algorithmic Complexity](#-algorithmic-complexity)
- [Project Structure](#-project-structure)
- [CLI Walkthrough & Sample Output](#-cli-walkthrough--sample-output)
- [Build & Run](#-build--run)
- [Future Enhancements](#-future-enhancements)

---

## 🏗 System Architecture

Indexra is organized around decoupled modules where each component fulfills a distinct responsibility within the Information Retrieval (IR) pipeline.

```mermaid
flowchart TB
    subgraph ClientLayer["🖥️ Presentation & Interface Layer"]
        CLI["CLI Driver & REPL<br/><code>src/main.cpp</code>"]
    end

    subgraph PreprocessingLayer["🔤 Lexical Analysis Layer"]
        TOK["Tokenizer<br/><code>src/tokenizer.cpp</code><br/><i>Normalization, Filtering, Case-folding</i>"]
    end

    subgraph StorageLayer["🗄️ Indexing & Storage Layer"]
        INV["Inverted Index<br/><code>src/inverted_index.cpp</code><br/><code>std::unordered_map&lt;string, std::unordered_map&lt;int, int&gt;&gt;</code>"]
        TRIE["Prefix Trie<br/><code>src/trie.cpp</code><br/><i>Fast Prefix Tree & DFS Exploration</i>"]
    end

    subgraph RetrievalLayer["⚡ Retrieval & Relevance Engine"]
        QP["Query Processor<br/><code>src/query_processor.cpp</code><br/><i>Boolean AND / OR Postings Evaluator</i>"]
        RE["Ranking Engine<br/><code>src/ranking_engine.cpp</code><br/><i>TF-IDF Scoring & Min-Heap Top-K Selection</i>"]
    end

    %% Ingestion Connections
    CLI -->|"1. Raw Corpus Documents"| TOK
    TOK -->|"2. Document Tokens"| INV
    TOK -->|"2. Document Tokens"| TRIE

    %% Query Connections
    CLI -->|"Autocomplete Request (/auto &lt;prefix&gt;)"| TRIE
    TRIE -->|"Word Suggestions"| CLI

    CLI -->|"Search Query String"| TOK
    TOK -->|"Query Tokens"| INV
    INV -->|"Per-Token Postings Lists"| QP
    QP -->|"Blended & Boosted Postings"| RE
    RE -->|"Ranked Top-K Search Results"| CLI

    classDef client fill:#e1f5fe,stroke:#0288d1,stroke-width:2px;
    classDef preprocess fill:#fff3e0,stroke:#f57c00,stroke-width:2px;
    classDef storage fill:#e8f5e9,stroke:#388e3c,stroke-width:2px;
    classDef retrieval fill:#ede7f6,stroke:#512da8,stroke-width:2px;

    class CLI client;
    class TOK preprocess;
    class INV,TRIE storage;
    class QP,RE retrieval;
```

---

## 🔄 System Lifecycle & Data Flow

Indexra operates in two discrete phases: **Ingestion & Indexing Time** and **Query & Retrieval Time**.

```mermaid
sequenceDiagram
    autonumber
    actor User as User / Terminal
    participant CLI as CLI Driver (main.cpp)
    participant Tok as Tokenizer
    participant Index as Inverted Index
    participant Trie as Trie
    participant QP as Query Processor
    participant Rank as Ranking Engine

    Note over User, Trie: Phase 1: Corpus Ingestion & Bootstrapping
    CLI->>Tok: tokenize(document_text)
    Tok-->>CLI: tokens [word1, word2, ...]
    CLI->>Index: addDocument(docId, tokens)
    Note right of Index: Maps word -> {docId: freq}
    CLI->>Trie: insert(token) for all tokens
    Note right of Trie: Builds prefix search tree

    Note over User, Rank: Phase 2: Interactive Query Processing
    User->>CLI: query ("data structures")
    CLI->>Tok: tokenize(query)
    Tok-->>CLI: queryTokens ["data", "structures"]
    loop For each token
        CLI->>Index: search(token)
        Index-->>CLI: postings {docId: term_freq}
    end
    CLI->>QP: processAndQuery(postingsLists)
    QP-->>CLI: andResults {docId: combined_freq}
    CLI->>QP: processOrQuery(postingsLists)
    QP-->>CLI: orResults {docId: combined_freq}
    Note over CLI: Blend results: OR_score + (AND_BOOST * AND_score)
    CLI->>Rank: rankTFIDF(blendedPostings, totalDocs, docsWithWord, k=5)
    Note right of Rank: Score = (1 + ln(TF)) * ln(1 + N/DF)<br/>Top-K via Min-Heap O(N log k)
    Rank-->>CLI: Top-K Vector of {docId, score}
    CLI-->>User: Formatted, ranked results table with execution latency
```

---

## ⚙️ How It Works: Component Deep-Dive

### 1. Tokenization & Normalization

The `Tokenizer` converts unstructured raw strings into normalized, indexable tokens:
- **Filtering:** Strips punctuation, control characters, and whitespace using `std::isalnum`.
- **Case-Folding:** Converts characters to lowercase with `std::tolower` for case-insensitive matching.
- **Pipeline Consistency:** Identical logic is shared across corpus indexing and user query evaluation to guarantee index alignment.

```
"Dynamic Programming is a powerful ALGorithmic technique!"
  │
  ▼ (Alphanumeric filter + std::tolower)
["dynamic", "programming", "is", "a", "powerful", "algorithmic", "technique"]
```

---

### 2. Inverted Index Construction

The `InvertedIndex` provides $O(1)$ average-time lookup for postings lists using a hash-map-of-hash-maps design:

$$\text{index}: \text{std::string} \longrightarrow \Big( \text{docId} \longrightarrow \text{frequency} \Big)$$

- Each unique term points to a map of document IDs and the term frequency within that document.
- Term frequencies serve as the foundational weight for downstream TF-IDF scoring.
- Average retrieval for a single keyword postings list is $O(1)$.

```mermaid
flowchart LR
    subgraph InvertedIndex["Inverted Index Structure"]
        direction TB
        subgraph Terms["Dictionary (Terms)"]
            T1["'algorithms'"]
            T2["'data'"]
            T3["'structures'"]
        end

        subgraph Postings1["Postings List: 'algorithms'"]
            P1A["Doc 0: freq = 1"]
            P1B["Doc 2: freq = 1"]
            P1C["Doc 12: freq = 1"]
        end

        subgraph Postings2["Postings List: 'data'"]
            P2A["Doc 0: freq = 1"]
            P2B["Doc 1: freq = 1"]
            P2C["Doc 4: freq = 1"]
            P2D["Doc 12: freq = 1"]
        end

        subgraph Postings3["Postings List: 'structures'"]
            P3A["Doc 0: freq = 1"]
            P3B["Doc 1: freq = 1"]
            P3C["Doc 7: freq = 1"]
            P3D["Doc 12: freq = 1"]
        end
    end

    T1 --> Postings1
    T2 --> Postings2
    T3 --> Postings3
```

---

### 3. Boolean Query Processor

Search queries frequently contain multiple terms. The `QueryProcessor` resolves boolean relationships between postings lists:

```mermaid
flowchart TD
    subgraph PostingsInput["Token Postings Lists"]
        PL1["Posting List T1<br/>{Doc0: 2, Doc1: 1, Doc3: 1}"]
        PL2["Posting List T2<br/>{Doc0: 1, Doc2: 3, Doc3: 2}"]
    end

    subgraph ANDLogic["AND Evaluation (Set Intersection)"]
        AND["Intersect DocIDs<br/>Early exit if result is empty<br/>Sum matching frequencies"]
        AND_OUT["AND Result<br/>{Doc0: 3, Doc3: 3}"]
    end

    subgraph ORLogic["OR Evaluation (Set Union)"]
        OR["Union DocIDs<br/>Accumulate all term frequencies"]
        OR_OUT["OR Result<br/>{Doc0: 3, Doc1: 1, Doc2: 3, Doc3: 3}"]
    end

    subgraph Blending["Hybrid Blending & Boosting"]
        BLEND["Blended Score Calculation<br/><code>Score = OR_Freq + (AND_BOOST * AND_Freq)</code><br/><i>(AND_BOOST = 10)</i>"]
        BLEND_OUT["Blended Candidate Postings<br/>{Doc0: 33, Doc1: 1, Doc2: 3, Doc3: 33}"]
    end

    PL1 --> AND
    PL2 --> AND
    AND --> AND_OUT

    PL1 --> OR
    PL2 --> OR
    OR --> OR_OUT

    AND_OUT -->|"Weight x 10"| BLEND
    OR_OUT -->|"Base Weight"| BLEND
    BLEND --> BLEND_OUT
```

- **AND Queries:** Computes the set intersection across all query term postings lists. If any term has zero matches or the intermediate intersection becomes empty, evaluation terminates early.
- **OR Queries:** Computes the set union, aggregating term occurrences across documents to ensure maximum recall.
- **Hybrid Blending with Priority Boosting:** Documents that satisfy all terms (AND match) receive a **10x boost** over documents containing only partial terms (OR matches):

$$\text{BlendedScore}(d) = \text{Freq}_{\text{OR}}(d) + \Big( 10 \times \text{Freq}_{\text{AND}}(d) \Big)$$

---

### 4. TF-IDF Ranking Engine & Top-K Selection

Documents matching query terms are ranked using a combination of **Term Frequency (TF)**, **Inverse Document Frequency (IDF)**, and **Heap-based selection**.

#### Mathematical Formulation

1. **Log-Normalized Term Frequency:**
   Diminishes the dominance of repetitive keywords within long documents:

   $$\text{TF}(t, d) = 1.0 + \ln(\text{rawTF}(t, d))$$

2. **Smoothed Inverse Document Frequency:**
   Penalizes words that appear across all documents while rewarding discriminative terms:

   $$\text{IDF}(t, D) = \ln\left(1.0 + \frac{N}{|d \in D : t \in d|}\right)$$

   Where $N$ is the total corpus size and $|d \in D : t \in d|$ is the document frequency ($DF$).

3. **Composite Document Score:**

   $$\text{Score}(d) = \text{TF}(t, d) \times \text{IDF}(t, D)$$

#### Min-Heap Top-K Selection

Instead of performing a full $O(N \log N)$ sort on the entire document space, the ranking engine maintains a **Min-Heap of size $K$** (`std::priority_queue` with `std::greater`):

```mermaid
flowchart TD
    Candidate["Candidate Document with Score (d, score)"] --> Check{"Heap size < K?"}
    Check -- Yes --> Push["Push (score, d) into Min-Heap"]
    Check -- No --> Compare{"score > heap.top()?"}
    Compare -- Yes --> PopPush["heap.pop()<br/>heap.push(score, d)"]
    Compare -- No --> Discard["Discard Candidate"]

    Push --> Done["Repeat for all Candidates"]
    PopPush --> Done
    Discard --> Done

    Done --> Extract["Drain Heap & Reverse<br/>Yields descending Top-K in O(N log K) time"]
```

---

### 5. Trie Autocomplete

The `Trie` module provides real-time search suggestions with prefix matching:

```mermaid
flowchart TD
    Root["Root Node (empty)"] --> A["'a'"]
    Root --> D["'d'"]
    
    D --> DA["'a'"]
    DA --> DAT["'t'"]
    DAT --> DATA["'a' (isEndOfWord = true) -> 'data'"]
    
    A --> AL["'l'"]
    AL --> ALG["'g' (Prefix Node)"]
    
    subgraph DFS["DFS Traversal from 'alg'"]
        ALG --> ALGO["'o'"]
        ALGO --> ALGOR["'r'"]
        ALGOR --> ALGORI["'i' ..."]
        ALGORI --> ALGORITHM["'m' (isEndOfWord = true) -> 'algorithm'"]
        ALGORITHM --> ALGORITHMS["'s' (isEndOfWord = true) -> 'algorithms'"]
    end

    classDef endWord fill:#c8e6c9,stroke:#2e7d32,stroke-width:2px;
    class DATA,ALGORITHM,ALGORITHMS endWord;
```

- **Prefix Locating:** Advances down the character tree in $O(L)$ time, where $L$ is the prefix length.
- **Subtree Exploration (DFS):** Recursively traverses all child branches to gather matching suffixes in $O(M)$ time, where $M$ is the number of nodes in the prefix subtree.
- **Fast Exit:** Immediately returns an empty result set if any character in the prefix does not exist in the trie.

---

## 📊 Algorithmic Complexity

| Component | Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Tokenizer** | Text tokenization | $O(L)$ where $L$ = string length | $O(T)$ where $T$ = token count |
| **Inverted Index** | Add document | $O(T)$ where $T$ = tokens in document | $O(V \times D)$ unique terms & postings |
| **Inverted Index** | Term lookup | $O(1)$ average | $O(P)$ where $P$ = postings list size |
| **Query Processor** | AND Intersection | $O(\min(P_1, P_2, \dots))$ with early exit | $O(\min(P_1, P_2, \dots))$ |
| **Query Processor** | OR Union | $O(\sum P_i)$ over all term postings | $O(\sum P_i)$ unique matching docs |
| **Ranking Engine** | Top-K Selection | $O(N \log K)$ ($N$ candidates, heap size $K$) | $O(K)$ heap space |
| **Trie** | Word insertion | $O(W)$ where $W$ = word length | $O(W \times |\Sigma|)$ |
| **Trie** | Prefix Autocomplete | $O(L + M)$ ($L$ = prefix len, $M$ = subtree nodes) | $O(S)$ where $S$ = suggestions size |

---

## 📁 Project Structure

```
Indexra/
├── CMakeLists.txt              # CMake build configuration (C++23)
├── .gitignore                  # Git ignore rules for build artifacts
├── README.md                   # Project documentation and architecture guide
└── src/
    ├── main.cpp                # Interactive REPL CLI and system pipeline orchestrator
    ├── tokenizer.h / .cpp      # String parsing, punctuation filtering & case normalization
    ├── inverted_index.h / .cpp # Postings list management and document mapping
    ├── query_processor.h / .cpp# Boolean logic evaluator (AND intersection & OR union)
    ├── ranking_engine.h / .cpp # TF-IDF calculation and Min-Heap Top-K ranking
    └── trie.h / .cpp           # Prefix tree data structure for autocomplete
```

---

## 🖥️ CLI Walkthrough & Sample Output

### Booting & Ingestion
```text
[SYSTEM] Booting Indexra and ingesting corpus...
[OK] Indexed 15 documents in 1.42 ms

  ___           _
 |_ _|_ __   __| | _____  ___ __ __ _
  | || '_ \ / _` |/ _ \ \/ / '__/ _` |
  | || | | | (_| |  __/>  <| | | (_| |
 |___|_| |_|\__,_|\___/_/\_\_|  \__,_|

 High-Performance Search Engine Core (C++23)
 Type your query, or type '/exit' to quit.
 Type '/auto <prefix>' for autocomplete demo.
==================================================
```

### Search Query Execution
```text
indexra> data structures

 [5 results found in 18.4 µs]
------------------------------------------------------------
  Rank DocID       Score  Snippet
------------------------------------------------------------
     1     0     32.7831  Data Structures and Algorithms form the core o...
     2    12     32.7831  algorithms and data structures are essential f...
     3     1     32.7831  advanced data structures like Segment trees an...
     4     7     32.7831  binary search trees heaps and tries are fundam...
     5     4     16.3916  competitive programming requires strong data s...
```

### Prefix Autocomplete
```text
indexra> /auto alg
 Autocomplete for 'alg':
   - algorithmic
   - algorithm
   - algorithms

indexra> /auto dat
 Autocomplete for 'dat':
   - data
```

---

## ▶️ Build & Run

### Prerequisites
- **Compiler:** Any modern C++23 compliant compiler (e.g. GCC 13+, Clang 16+, or MSVC 2022 v19.34+)
- **Build System:** CMake 3.10+ (optional, direct g++ compilation also supported)

### Option 1: Build with CMake (Recommended)

```bash
# Generate build files
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build the executable
cmake --build build --config Release

# Run Indexra
./build/indexra          # On Linux / macOS
./build/Release/indexra  # On Windows (MSVC)
```

### Option 2: Direct Compilation with g++

```bash
cd src
g++ -std=c++23 -O3 -Wall -Wextra main.cpp tokenizer.cpp inverted_index.cpp \
    query_processor.cpp ranking_engine.cpp trie.cpp -o indexra

# Execute
./indexra
```

---

## 🚀 Future Enhancements

- [ ] **BM25 Relevance Scoring:** Implement Okapi BM25 ranking with document length normalization ($k_1$, $b$ parameters).
- [ ] **Positional Indexing:** Track term positions within documents to enable exact phrase queries (`"quick brown fox"`) and proximity searching.
- [ ] **Persistence Layer:** Serialize and deserialize inverted index and trie structures to disk using memory-mapped files or protocol buffers.
- [ ] **Fuzzy Search:** Integrate Levenshtein Automaton with the Trie to support spell-checking and typo tolerance.
- [ ] **Multi-threaded Ingestion:** Implement chunk-based parallel indexing using a producer-consumer thread pool.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
