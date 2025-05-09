# collision-detection-optimization

## Contents

1. [Overview](#overview)
2. [Compilation](#compilation)
3. [Disclaimer](#disclaimer)
4. [Technical Details](#technical-details)
	1. [Initial Setup](#initial-setup)
	2. [Rendering](#rendering)
	3. [Physics and Collisions](#physics-and-collisions)
	4. [Spatial Partitioning](#spatial-partitioning)
	5. [Multithreading](#multithreading)
	6. [Compiler Optimization](#compiler-optimization)
5. [What I learned](#what-i-learned)
6. [Cool Stuff](#cool-stuff)

## Overview

This project is basically a bunch of circles getting the opportunity to experience physics thanks to Verlet integration and a custom collision solver. Scroll down for a litte detour on how I made it and more importantly how I made it faster.

## Disclaimer

Shoutout to Jean Tampon for the awesome [project](https://github.com/johnBuffer/VerletSFML-Multithread) and [video](https://youtu.be/9IULfQH7E90?si=OxRCw2NqhMKmwj62) that inspired me! I wanted to see if I could pull off something similar while practicing my C++ and OpenGL skills for university classes. Most of it is built from scratch, but I did borrow a bit of wisdom—especially around Verlet integration.

## Compilation

Ensure [CMake](https://cmake.org), [GLFW](https://www.glfw.org), and [GLEW](https://glew.sourceforge.net) are installed. The project bundles [ImGui](https://github.com/ocornut/imgui), [glm](https://github.com/g-truc/glm), and [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h).

To build run the following commands:

	mkdir build
	cd build
	cmake .. -DCMAKE_CXX_FLAGS="-O3"
	cmake --build .

Place the `res` directory alongside the executable (this is handled automatically during build).

## Technical Details

Performance benchmarks were run to measure how many objects the simulation supports before dropping below 60 fps. Tests were run on my personal laptop without compiler optimizations unless otherwise noted. Solely to get a rough comparison of performance for each step.

### Initial Setup

The baseline system included rendering, gravity, velocity, and scene management, but no collisions. Each circle had its own draw call, maxing out around 5500 objects due to CPU overhead.

### Rendering

Switching to instanced rendering allowed letting the GPU do the heavy lifting and packing everything into one draw call. Using `glMapBufferRange`, object transforms are uploaded efficiently to the GPU, increasing support to over 100,000 objects.

### Physics and Collisions

The initial implementation used semi-implicit Euler integration but switched to Verlet integration for stability (and I didn’t know that existed at the beginning). A naive O(n^2) pairwise collision check limited support to \~580 objects.

### Spatial Partitioning

Dividing the world into a uniform grid (cells sized to circle diameter) improved performance drastically. Each object is now checked only against neighbors in its own cell and the surrounding 8. This allowed \~6600 objects — a 11× improvement.

### Multithreading

The grid-based collision detection was parallelized by assigning non-overlapping grid slices to threads. A two-pass approach prevents data races by ensuring threads process alternating slices. This raised capacity to \~21,600 objects using 10 threads.

### Compiler Optimization

With `-O3`, the simulation handled over 110,000 objects. Turns out, with a bit of compiler fairy dust, the simulation runs a lot better. Performance bottlenecks then shifted to grid overhead. For example, at 110,000 objects, increasing the grid from 600×600 to 620×620 grid cells, the frame rate dropped from ~140 to ~60.

## What I learned

As someone still new to C++, this project was a great learning experience in both the language and its quirks. I also just started a university class on OpenGL, so this gave me a chance to get hands-on with rendering basics and even dip into instanced rendering. On the physics side, I learned how simple simulations work in games and how to handle collisions between primitives. Implementing spatial partitioning for the first time was a nice practice for basic game development skills.

## Cool Stuff

Due to the deterministic nature of the simulation it supports creative experiments—e.g., assigning colors to the circles based on an image and replaying the same simulation to display the image using the circles and seeing how it slowly comes together.
You can also explore other effects by setting the gravity to a single point in space or changing the interaction rules.

---

Feel free to try your own variations!