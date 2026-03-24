# API Reference

## Core Classes

### IWindow (Abstract Interface)

Platform-independent window interface.

#### Methods

| Method | Description |
|--------|-------------|
| `Create(width, height, title)` | Create window with specified size |
| `Destroy()` | Destroy window and cleanup |
| `PollEvents()` | Process window messages |
| `SwapBuffers()` | Swap front/back buffers |
| `IsClosed()` | Check if window should close |
| `MakeContextCurrent()` | Make OpenGL context current |
| `IsKeyPressed(key)` | Check if key is pressed |
| `GetMousePosition(x, y)` | Get current mouse position |
| `SetVSync(enabled)` | Enable/disable VSync |

### Renderer

Main rendering class that handles game loop and OpenGL.

#### Methods

| Method | Description |
|--------|-------------|
| `Initialize(width, height, title)` | Initialize renderer and window |
| `Shutdown()` | Cleanup resources |
| `Run()` | Start main game loop |
| `Clear(r, g, b)` | Clear screen with color |
| `DrawRect(x, y, w, h)` | Draw rectangle |
| `DrawRect(x, y, w, h, r, g, b)` | Draw colored rectangle |
| `SwapBuffers()` | Present rendered frame |
| `ShouldClose()` | Check if window should close |
| `GetWindow()` | Get window pointer |

#### Virtual Methods (Override in game)

| Method | Description |
|--------|-------------|
| `OnUpdate(deltaTime)` | Update game logic |
| `OnRender()` | Render graphics |
| `OnKeyDown(key)` | Handle key press |
| `OnKeyUp(key)` | Handle key release |
| `OnMouseMove(x, y)` | Handle mouse movement |
| `OnResize(width, height)` | Handle window resize |

## Key Codes

Platform-independent key codes are mapped in `Platform.h`:

```cpp
namespace Engine::Key {
    constexpr KeyCode Space = 32;
    constexpr KeyCode Escape = 27;
    constexpr KeyCode Up = 38;
    constexpr KeyCode Down = 40;
    constexpr KeyCode Left = 37;
    constexpr KeyCode Right = 39;
}
