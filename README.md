# 🌌 Raytracer – B-OOP-400

> “Your CPU goes brrrrrr…”

---

## 🚀 Project Ambition

The goal of this project is to build a **modular, extensible, and realistic raytracer** in C++, following strong OOP principles, clean architecture, and proven design patterns.

This engine simulates the physical behavior of light in a 3D scene and produces images with **accurate geometry**, **material interaction**, and **lighting**. Scenes are described via external configuration files, and the core is designed for **runtime extensibility** through **dynamic plugins**.

We’re not just casting rays; we’re crafting a framework ready for serious extensions and professional-grade ray tracing.

---

## 📖 Documentation

Complete API documentation (Doxygen) is available here:

[https://flogrv.github.io/B-OOP-400-NCY-4-1-raytracer-theo.meligner/](https://flogrv.github.io/B-OOP-400-NCY-4-1-raytracer-theo.meligner/)

It includes:

* Full class hierarchy and namespaces
* Detailed API reference for all modules
* Implementation notes and usage examples

---

## 📋 Technical Specifications (Requirements)

### ✅ Mandatory Features

* **Primitives**: Sphere, Plane, Triangle, Cone, Cylinder, .OBJ loader
* **Lighting**: Ambient and Directional lights
* **Camera**: Configurable position, rotation, FOV; multiple cameras supported
* **Transformations**: Translation
* **Materials**: Flat color shading
* **Scene**: External `.cfg` configuration files
* **Plugins**: Dynamic `.so` loader for primitives and lights
* **CLI**: Command-line interface for interactive control
* **Output**: PPM image files; no built-in GUI required
* **Architecture**: Use of interfaces for primitives & lights; at least two design patterns (Factory, Composite)

### 🧠 Optional Features (Should/Could-have)

* Anti-aliasing (supersampling)
* Shadows (hard, soft), reflections, textures
* Rotation, scaling, shearing transforms
* Multithreading optimizations
* Real-time SFML viewer

---

## ⚙️ Compilation & Execution

```bash
# From project root:
mkdir build
cd build
cmake ..
make -j
cd ..
```

* **Executable**: `raytracer` is placed in the project root.
* **Plugins**: compiled into `plugins/`
* **Scenes**: .cfg files in `scenes/`
* **Output**: .ppm images saved to `screenshots/`

Run:

```bash
./raytracer scenes/<scene_name>.cfg
```

This launches an interactive CLI.

---

## 🖥️ Command-Line Interface Commands

Once `raytracer` is running, use:

```text
move <object> <dx> <dy> <dz>   # Translate object or camera by vector
cam <camera_name>              # Switch to named camera
render                         # Render current view to a .ppm file in screenshots/
preview                        # Display the last rendered frame in an SFML window
exit                           # Quit the CLI
```

---

## 📦 Prerequisites

* Linux or WSL
* C++20 compiler (`g++`, `clang++`)
* CMake ≥ 3.16
* `libconfig++` for scene parsing
* SFML 2.5 (optional, for real-time viewer)

Install dependencies on Ubuntu/Debian:

```bash
sudo apt update && sudo apt install cmake g++ libconfig++-dev libsfml-dev
```

---

## 🎉 Credits

Principal developers:

* @CMoiMec
* @Flogrv
* @Hablio
* @theomeligner

---
