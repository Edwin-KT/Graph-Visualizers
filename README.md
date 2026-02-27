# Graph-Visualizers

# Graph Visualizers

A collection of C++ desktop applications built with the Qt framework, designed to visualize and interact with various graph algorithms.

The repository is divided into three distinct projects, each focusing on a different category of graph problems:

## 1. Dijkstra Route Planner
A navigation system based on a real-world road network.
- Loads and renders a map from an XML file (e.g., Luxembourg map).
- Allows user interaction through panning and zooming.
- Uses a KD-Tree data structure for fast nearest-node lookup based on cursor position.
- Implements Dijkstra's algorithm to calculate and visualize the shortest path between two user-selected points.

## 2. Floyd-Warshall, Kruskal & TSP Visualizer
An application for demonstrating classic optimization and routing algorithms.
- Loads graphs from a text file (cities and distances).
- **Floyd-Warshall**: Calculates the shortest paths between all pairs of nodes.
- **Kruskal**: Finds the Minimum Spanning Tree (MST).
- **TSP (Traveling Salesperson Problem)**: Approximates the minimum cost Hamiltonian cycle using a preorder traversal of the resulting MST.

## 3. Ford-Fulkerson Visualizer
A visualizer for the maximum flow problem in a network.
- Graphically represents a network with edge capacities.
- Applies the Ford-Fulkerson algorithm to find and illustrate the maximum flow from a source to a sink node.

## Technologies Used
- C++ (C++17 Standard)
- Qt 5 / Qt 6 (Core, GUI, Widgets)

## Build and Run
1. Clone this repository.
2. Open the desired project's `.pro` file (e.g., `DijkstraRoutePlanner.pro`) using Qt Creator.
3. Select your installed build kit (MinGW or MSVC).
4. Ensure the required data files (`.xml` or `.txt`) are accessible at runtime (some projects allow loading them directly via the UI).
5. Build and Run.
