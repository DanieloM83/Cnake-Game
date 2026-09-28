# Cnake Game

**A classic snake game implemented in C using [raylib](https://www.raylib.com/).**

<div align="left">
<img alt="Game-Demo" src="https://github.com/user-attachments/assets/fe60733c-4cfa-4991-a551-6be0a6c328f9" />
</div>

## Get started

### Installation

Go to the [Releases](https://github.com/DanieloM83/Cnake-Game/releases) page and download the latest version for your operating system.

### Manual build

Run the commands below from the repository root. **CMake 3.25+**, a **C11 compiler**, and an internet connection are required. Raylib is downloaded automatically during the first configuration.

```bash
git clone https://github.com/DanieloM83/Cnake-Game.git
cd Cnake-Game
```

#### Linux (ubuntu)

Install the required dependencies:
```bash
sudo apt install cmake build-essential \
  libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev \
  libxi-dev libxext-dev libgl1-mesa-dev libasound2-dev
```
Build and run the game:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/cnake
```

#### macOS

Install [Xcode Command Line Tools](https://developer.apple.com/documentation/xcode/installing-the-command-line-tools) and [CMake](https://cmake.org/download/), then run:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/cnake
```

#### Windows

Install Visual Studio with the **Desktop development with C++** workload and [CMake](https://cmake.org/download/). From PowerShell, run:
```bash
cmake -S . -B build
cmake --build build --config Release
.\build\Release\cnake.exe
```

## Project structure

```plaintext
cnake-game/
├── .github
│   └── workflows
│       ├── cd.yaml  # Creates releases from Git tags
│       └── ci.yaml  # Builds and smoke-tests the project
└── src
    ├── cnake.h      # Shared declarations
    ├── draw.c       # Rendering logic
    ├── game.c       # Game state and gameplay logic
    ├── main.c       # Application entry point
    └── snake.c      # Snake movement and collisions
```

## CI/CD

GitHub Actions builds and smoke-tests the project on Linux, macOS and Windows.
Releases are created automatically when a version tag is pushed.

## License

This project is licensed under the MIT License.
