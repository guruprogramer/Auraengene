/**
 * Minimalist 2D Game Engine
 * 
 * A minimal 2D game engine built with pure Win32 API and GDI.
 * No external dependencies, just Windows SDK.
 * 
 * Features:
 * - Double buffering for smooth rendering
 * - Stable FPS with delta time
 * - Keyboard input (WASD/Arrow keys)
 * - Boundary collision detection
 * - FPS counter
 * 
 * Compilation:
 *   MSVC: cl /EHsc /O2 /SUBSYSTEM:WINDOWS main.cpp user32.lib gdi32.lib
 *   MinGW: g++ -O2 -static -mwindows -lgdi32 main.cpp -o game.exe
 * 
 * Author: Your Name
 * License: MIT
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <cstdlib>

#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

// ==================== Game Constants ====================
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int PLAYER_SIZE = 50;
constexpr float PLAYER_SPEED = 300.0f; // units per second

// Simple RGB color structure
struct Color {
    BYTE r, g, b;
    Color(BYTE red, BYTE green, BYTE blue) : r(red), g(green), b(blue) {}
};

// ==================== Game Object ====================
class GameObject {
public:
    float x, y;
    int width, height;
    Color color;
    
    GameObject(int x, int y, int w, int h, Color col) 
        : x(static_cast<float>(x)), y(static_cast<float>(y)), width(w), height(h), color(col) {}
    
    void Draw(HDC hdc) {
        HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
        RECT rect = { static_cast<int>(x), static_cast<int>(y), 
                      static_cast<int>(x + width), static_cast<int>(y + height) };
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }
};

// Forward declaration
class GameEngine;

// Global pointer for window procedure
GameEngine* g_pEngine = nullptr;

// ==================== Game Engine Class ====================
class GameEngine {
private:
    HWND hwnd;
    HBITMAP hbmMem;
    HDC hdcBackBuffer;
    HDC hdcScreen;
    
    bool running;
    float deltaTime;
    
    LARGE_INTEGER perfFreq;
    LARGE_INTEGER lastTime;
    float targetFrameTime;
    float fpsTimer;
    int frameCount;
    int currentFPS;
    
    GameObject* player;
    bool keys[256];
    
    void ProcessInput() {
        float moveAmount = PLAYER_SPEED * deltaTime;
        
        if (keys['W'] || keys[VK_UP]) {
            player->y -= moveAmount;
        }
        if (keys['S'] || keys[VK_DOWN]) {
            player->y += moveAmount;
        }
        if (keys['A'] || keys[VK_LEFT]) {
            player->x -= moveAmount;
        }
        if (keys['D'] || keys[VK_RIGHT]) {
            player->x += moveAmount;
        }
        
        // Boundary collision
        if (player->x < 0) player->x = 0;
        if (player->y < 0) player->y = 0;
        if (player->x + player->width > WINDOW_WIDTH) 
            player->x = WINDOW_WIDTH - player->width;
        if (player->y + player->height > WINDOW_HEIGHT) 
            player->y = WINDOW_HEIGHT - player->height;
    }
    
    void Update() {
        fpsTimer += deltaTime;
        frameCount++;
        if (fpsTimer >= 1.0f) {
            currentFPS = frameCount;
            frameCount = 0;
            fpsTimer = 0.0f;
        }
    }
    
    void Render() {
        if (!hwnd || !hdcBackBuffer) return;
        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        
        if (hdc && hdcBackBuffer) {
            // Clear back buffer
            RECT rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
            HBRUSH bgBrush = CreateSolidBrush(RGB(30, 30, 45));
            FillRect(hdcBackBuffer, &rect, bgBrush);
            DeleteObject(bgBrush);
            
            // Draw player
            if (player) {
                player->Draw(hdcBackBuffer);
            }
            
            // Draw UI text
            char fpsText[64];
            sprintf_s(fpsText, "FPS: %d", currentFPS);
            SetBkMode(hdcBackBuffer, TRANSPARENT);
            SetTextColor(hdcBackBuffer, RGB(255, 255, 255));
            TextOutA(hdcBackBuffer, 10, 10, fpsText, static_cast<int>(strlen(fpsText)));
            
            char controlsText[] = "Controls: WASD or Arrow Keys";
            TextOutA(hdcBackBuffer, 10, 35, controlsText, static_cast<int>(strlen(controlsText)));
            
            char positionText[64];
            if (player) {
                sprintf_s(positionText, "Position: (%.0f, %.0f)", player->x, player->y);
                TextOutA(hdcBackBuffer, 10, 60, positionText, static_cast<int>(strlen(positionText)));
            }
            
            // Copy to screen (double buffering)
            BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdcBackBuffer, 0, 0, SRCCOPY);
        }
        
        EndPaint(hwnd, &ps);
    }
    
    void CleanupRenderResources() {
        if (hbmMem) {
            DeleteObject(hbmMem);
            hbmMem = nullptr;
        }
        if (hdcBackBuffer) {
            DeleteDC(hdcBackBuffer);
            hdcBackBuffer = nullptr;
        }
        if (hdcScreen) {
            ReleaseDC(hwnd, hdcScreen);
            hdcScreen = nullptr;
        }
    }
    
public:
    GameEngine() : hwnd(nullptr), hbmMem(nullptr), 
                   hdcBackBuffer(nullptr), hdcScreen(nullptr),
                   running(false), deltaTime(0), targetFrameTime(1.0f / 60.0f),
                   fpsTimer(0), frameCount(0), currentFPS(0),
                   player(nullptr) {
        ZeroMemory(keys, sizeof(keys));
    }
    
    ~GameEngine() {
        Shutdown();
    }
    
    bool Initialize(HINSTANCE hInstance, int nCmdShow) {
        // Register window class
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"GameEngineClass";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        
        if (!RegisterClassEx(&wc)) {
            return false;
        }
        
        // Create window
        hwnd = CreateWindowEx(
            0,
            L"GameEngineClass",
            L"Minimalist 2D Game Engine",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            WINDOW_WIDTH, WINDOW_HEIGHT,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );
        
        if (!hwnd) {
            return false;
        }
        
        g_pEngine = this;
        
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
        
        // Create back buffer for double buffering
        hdcScreen = GetDC(hwnd);
        if (!hdcScreen) {
            return false;
        }
        
        hdcBackBuffer = CreateCompatibleDC(hdcScreen);
        if (!hdcBackBuffer) {
            ReleaseDC(hwnd, hdcScreen);
            return false;
        }
        
        hbmMem = CreateCompatibleBitmap(hdcScreen, WINDOW_WIDTH, WINDOW_HEIGHT);
        if (!hbmMem) {
            DeleteDC(hdcBackBuffer);
            ReleaseDC(hwnd, hdcScreen);
            return false;
        }
        
        SelectObject(hdcBackBuffer, hbmMem);
        
        // Initialize player
        player = new GameObject(WINDOW_WIDTH / 2 - PLAYER_SIZE / 2, 
                                WINDOW_HEIGHT / 2 - PLAYER_SIZE / 2, 
                                PLAYER_SIZE, PLAYER_SIZE, 
                                Color(0, 255, 0));
        
        // Initialize performance counters
        QueryPerformanceFrequency(&perfFreq);
        QueryPerformanceCounter(&lastTime);
        
        running = true;
        return true;
    }
    
    void Run() {
        MSG msg = {};
        
        while (running) {
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    running = false;
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            
            if (!running) break;
            
            LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
            
            float frameTime = static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / perfFreq.QuadPart;
            lastTime = currentTime;
            deltaTime = min(frameTime, 0.033f);
            
            ProcessInput();
            Update();
            
            if (hwnd) {
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateWindow(hwnd);
            }
            
            float frameDuration = static_cast<float>(currentTime.QuadPart - lastTime.QuadPart) / perfFreq.QuadPart;
            if (frameDuration < targetFrameTime) {
                DWORD sleepTime = static_cast<DWORD>((targetFrameTime - frameDuration) * 1000);
                if (sleepTime > 0) {
                    Sleep(sleepTime);
                }
            }
        }
    }
    
    void Shutdown() {
        running = false;
        
        delete player;
        player = nullptr;
        
        CleanupRenderResources();
        
        if (hwnd) {
            DestroyWindow(hwnd);
            hwnd = nullptr;
        }
        
        g_pEngine = nullptr;
    }
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        GameEngine* engine = g_pEngine;
        
        switch (msg) {
            case WM_KEYDOWN:
                if (engine && wParam < 256) {
                    engine->keys[wParam] = true;
                }
                return 0;
                
            case WM_KEYUP:
                if (engine && wParam < 256) {
                    engine->keys[wParam] = false;
                }
                return 0;
                
            case WM_CLOSE:
                if (engine) {
                    engine->running = false;
                }
                DestroyWindow(hwnd);
                return 0;
                
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
                
            case WM_PAINT:
                if (engine) {
                    engine->Render();
                } else {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);
                    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
                    EndPaint(hwnd, &ps);
                }
                return 0;
        }
        
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
};

// ==================== Entry Point ====================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    GameEngine engine;
    
    if (!engine.Initialize(hInstance, nCmdShow)) {
        MessageBoxA(nullptr, "Failed to initialize game engine!", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
    
    engine.Run();
    engine.Shutdown();
    
    return 0;
}