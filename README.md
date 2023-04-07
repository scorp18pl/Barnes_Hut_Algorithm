# Barnes_Hut_Algorithm

This project simulates gravity force interactions between many entities. To this end it
implements the [Barnes Hut Algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) which uses quadtrees
to achieve `O(n log n)` time complexity.
This project also allows the user to compare it with the brute force algorithm
that has a time complexity of `O(n^2)`. The goal of this project was practicing the C++ development
and data structures (as well as playing with some basic physics).
It also implements a few tools for visualising the inner workings of the algorithm.

![](static/preview.gif)

## Preview Video

You can find the preview video of this project at [this link](https://www.youtube.com/watch?v=C6pCYyDt_3o&ab_channel=Scorp).

## Controls

* Camera movement
    - `W` - Move up
    - `A` - Move left
    - `S` - Move down
    - `D` - Move right

  You can control camera zoom using the mouse scroll button.

More options are available in the GUI.

## Preset System

The Preset System offers a way to customize your simulation. Each preset
describes the simulated entities. You can access the Simulation Preset Editor
through GUI. In the Simulation Preset Editor you can add new entities to the preset,
customize currently existing ones and save the preset for future use.

Currently, there are four built-in simulation presets:

- Default - One big entity in the middle with thousands smaller ones around it.
- TwoBody, ThreeBody, FiveBody - A multiple body systems of entities orbiting around the system's center of mass.

***Note:*** *Due to the fact that the BarnesHut Algorithm is just an approximation of actual physics, floating point
arithmetic limitations and the chaotic nature of n-body systems the provided systems may over time break their
symmetry.*

## Requirements

- The [Universal](https://github.com/scorp18pl/Universal) library installed.
- The [SFML](hhttps://github.com/SFML/SFML) library installed.
- The [ImGui-SFML](https://github.com/SFML/imgui-sfml) library installed.

## Setup

First, clone the repository using

```console
$ git clone https://github.com/scorp18pl/Barnes_Hut_Algorithm
```

Then you will have to install sfml

```console
$ sudo apt-get install libsfml-dev
```

After that create a build folder (inside the root repo folder) and head to it

```console
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release .. && make
```

The bha file inside the build directory is the binary
used to run the program.
