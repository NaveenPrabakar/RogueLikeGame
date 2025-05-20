**1.01 Dungeon Generation (C)**

- Created procedural dungeons with randomized rooms and corridors
- Implemented rock hardness values (0-255) for tunneling mechanics
- Used dynamic arrays for room storage

**1.02 File I/O System (C)**

- Developed cross-compatible binary format for dungeon saving/loading
- Added checksum validation for file integrity
- Implemented endianness conversion for portability

**1.03 Pathfinding (C)**

- Built two navigation systems:
    - BFS for non-tunneling NPCs
    - Dijkstra's Algorithm with rock hardness weights for tunnelers
- Optimized priority queue using binary heap

**1.04 Combat System (C)**

- Designed collision-based combat with instant death mechanics
- Implemented AI behavior flags (aggressive, passive, erratic)
- Created deterministic turn order system

**1.05 Curses Interface (C)**

- Added keyboard-controlled PC movement
- Implemented line-of-sight calculations
- Designed color-coded tile display with status HUD

**1.06 C++ Transition**

- Converted core structures to C++ classes without inheritance
- Maintained C-style memory management during transition
- Added fog-of-war system with dual map layers

**1.07-1.08 Data Parsing**

- Created custom text parser for monster attributes
- Implemented factory pattern for NPC generation

**1.09 Inventory System (C++)**

- Developed equipment system using enum-based slots
- Added item swap mechanics for ground/equipped items
- Transitioned combat to use damage dice and HP

**1.10 Audio Integration**

- Added mpg123 for background music playback
- Implemented positional sound effects
- Used system calls to play audio through the game


