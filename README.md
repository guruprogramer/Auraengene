# Minimalist 2D Game Engine

Aura game engine built with pure Win32 API and GDI. No external dependencies, just the Windows SDK.

![Game Engine Demo](screenshot.png)

## Features

- 🎮 **Pure Win32 API** - No external libraries required
- 🖼️ **Double Buffering** - Smooth rendering without flickering
- ⚡ **Stable FPS** - Delta time based movement with frame rate limiting
- ⌨️ **Keyboard Input** - WASD and Arrow key controls
- 🎯 **Collision Detection** - Boundary collision for game objects
- 📊 **Real-time FPS Counter** - Monitor performance
- 🟢 **Example Player** - Moveable green square

## Requirements

- Windows 7 or later
- Visual Studio 2019/2022 or MinGW-w64
- Windows SDK (comes with Visual Studio)

## Compilation

### Visual Studio Command Line
```bash
cl /EHsc /O2 /SUBSYSTEM:WINDOWS src/main.cpp user32.lib gdi32.lib