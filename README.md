# Barnes_Hut_Algorithm
This project implements the `Barnes Hut Algorithm` that uses quadtrees to achieve
`O(n log n)` time complexity. It allows for comparison with the brute force algorithm 
with time complexity `O(n^2)`. The goal of this project was practicing the C++ development
and data structures.
It also implements a few tools for visualising the inner workings of the algorithm.

# Controls

* General
    - `B` - toggle between the `Barnes Hut` and the brute force algorithm
* Camera movement
    - `W` - move up
    - `A` - move left
    - `S` - move down
    - `D` - move right
    - `P` - zoom in
    - `L` - zoom out
* Follow option
    - `E` - follow the next entity
    - `Q` - follow the previous entity
    - `Z` - disable the follow mode
* Visuals
    - `T` - toggle the quad - tree visual
    - `F` - toggle `the acceleration (red)`, `velocity (green)` vector visuals
    - `G` - toggle entity trackers
    - `C` - clear the entity trackers
    - `I` - toggle the entity tracker point limit
