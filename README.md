# Graph-Based Pac-Man in C++

A complete, interactive Pac-Man clone developed in C++ using the Simple Graphics Library (SGG). This project was developed as part of the "Computer Programming in C++" course at the Athens University of Economics and Business (AUEB).

The primary focus of this project is software architecture, emphasizing robust Object-Oriented Programming (OOP), graph theory for entity movement, and efficient algorithmic pathfinding.

## 🏗️ Architecture & Implementation Highlights

* **Graph-Based Movement:** Instead of a traditional 2D grid matrix, the maze is constructed as a network of nodes (`GameGraph`). Entities navigate by validating edges between nodes.
* **Algorithmic Pathfinding (AI):** The ghost AI utilizes the **Breadth-First Search (BFS)** algorithm to calculate the optimal path to Pac-Man dynamically.
* **Polymorphism & Inheritance:** Game entities (Pac-Man, Ghosts) inherit from a base `Character` class, enabling polymorphic method calls.
* **Singleton Design Pattern:** The game loop, state progression, and global asset management are strictly controlled through a single `GlobalState` instance.
* **Dynamic Memory Management:** Smart pointers (`std::shared_ptr`, `std::unique_ptr`) are utilized extensively to prevent memory leaks during runtime.

## 🚀 How to Build and Run

**⚠️ Important:** This repository does NOT include the pre-compiled SGG (Simple Graphics Library) binaries (`.lib`, `.dll`, `.so`) as they are intentionally excluded via `.gitignore` to keep the repository clean.

To compile and run this project locally, you must manually provide the SGG library to your build environment:

1. **Download SGG:** Obtain the Simple Graphics Library (SGG) files provided by the AUEB course.
2. **Add Library Files:** Place the `lib` folder (containing the required `.lib` files) into the root directory of this project.
3. **Add Header Files:** Ensure the `include/sgg` folder contains `graphics.h` and `scancodes.h`.
4. **Add Dynamic Libraries:** Copy the necessary `.dll` files (e.g., `sgg.dll`, `freetype.dll`, `glew32.dll`, `SDL2.dll`) into your output directory (e.g., inside `bin/` or `x64/Debug/`), so the executable can find them at runtime.
5. **Assets:** Ensure that the `assets/` folder (containing images, fonts, and sound files) is located in the working directory of the executable.
6. **Compile:** Open the `PacManGame.sln` in Visual Studio, build the solution, and run.
