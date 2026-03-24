# Cross-Platform Game Engine

A lightweight, cross-platform game engine written in C++ with OpenGL. Features clean separation of platform-specific code, allowing easy porting between Windows, Linux, and macOS.

[![Windows](https://img.shields.io/badge/Windows-0078D6?style=flat&logo=windows&logoColor=white)](https://github.com)
[![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)](https://github.com)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

## Features

- 🎮 **Pure C++17** - Modern C++ with smart pointers and RAII
- 🖥️ **Cross-Platform** - Same code runs on Windows, Linux, and macOS
- 🎨 **OpenGL Rendering** - Hardware-accelerated 2D graphics
- 🔌 **Clean Architecture** - Platform code separated from game logic
- 🏭 **Factory Pattern** - Platform detection at compile time
- 🚀 **No External Dependencies** - Uses only native APIs (Win32, X11, Cocoa)
- 📦 **Header-Only Interface** - User code sees only abstract interfaces

## Supported Platforms

| Platform | Status | Notes |
|----------|--------|-------|
| Windows  | ✅ Complete | Win32 API + WGL |
| Linux    | 🚧 Planned | X11 + GLX |
| macOS    | 🚧 Planned | Cocoa + CGL |

## Quick Start

### Windows (Visual Studio)

```bash
# Clone the repository
git clone https://github.com/yourusername/CrossPlatformGameEngine.git
cd CrossPlatformGameEngine

# Build with CMake
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release

# Run the example
./bin/Release/SimpleGame.exe