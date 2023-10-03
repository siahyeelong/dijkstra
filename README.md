# Dijkstra's Algorithm Implementation

This repository contains the implementation of Dijkstra's algorithm in C, developed as part of SC2001 Exercise 2.

## Repository Structure

- **`dijkstras_alg.c`**: The main code that performs Dijkstra's Algorithm.
- **`dijkstras_alg_analysis.c`**: The main code that analyzes the runtime of both list and matrix representations for Dijkstra's Algorithm.
- **`generate_graph.c`**: Use this to generate a graph of size V to test out Dijkstra's Algorithm
- **`references`**: A folder containing all the reference materials used.
- **`INPUT`**: Files with this prefix are used as inputs for the main code.
- **`OUTPUT`**: Files with this prefix are used as outputs for the main code.

Feel free to explore the code and references to understand the implementation and analysis of Dijkstra's algorithm.

## Usage

You can use the following steps to utilize this code:

1. Clone this repository to your local machine
2. Compile the code using your preferred C compiler
3. Run `dijkstras_alg.c` to perform Dijkstra's algorithm and check for the implemented algorithm's correctness
4. Generate a graph of size V using `generate_graph.c` by outputting the results into `INPUT_test_graph.txt`

```
... > INPUT_test_graph.txt
```

5. Run `dijkstras_alg_analysis.c` to analyse the runtime for both list and matrix representation by setting the input to be the `INPUT_test_graph.txt` file you just made

```
... < INPUT_test_graph.txt
```

## References

In the `references` folder, you'll find:

1. **`generate graphs for dijstra.xlsx`**: Excel sheet that generates the graph input text. Input V and you will get an array which represents a directed graph with weights
2. **`graph_representation.c`**: Was part of SC1007 assignment. Used as reference for graph representation in c
3. **`priority_queue.c`**: Used as a test bench for priority queue implemented in c

---

By Siah Yee Long
