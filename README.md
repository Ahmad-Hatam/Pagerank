This repository contains an implementation of the PageRank algorithm, a fundamental algorithm used by search engines to rank web pages in search results. The project includes functions for parsing graph data, simulating random surfers, and computing PageRank using both the random surfer and Markov chain models.

Features:
Graph Parsing: Efficiently parse graph data from .dot files.
Random Surfer Model: Simulate a random surfer to estimate page ranks.
Markov Chain Model: Compute PageRank using iterative matrix multiplication.
Statistical Analysis: Collect and analyze graph statistics such as in-degrees and out-degrees.
Memory Safety: Enhanced with AddressSanitizer for detecting memory errors.

Technologies:
1. C programming language
2. Makefile for build automation
3. AddressSanitizer for memory leak detection

Usage:
1. Clone the repository.
2. Build the project using make.
3. Run the PageRank computation on your graph data.

Goals:
1. Understand and implement the core concepts of the PageRank algorithm.
2. Provide a reliable and efficient tool for academic and educational purposes.
3. Ensure robust memory management and performance optimization.

Feel free to explore the code, report issues, and contribute to the project!
