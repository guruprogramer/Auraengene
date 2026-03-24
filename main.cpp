/**
 * main.cpp - Example usage of the engine
 * This code is completely platform independent!
 */

#include "Core/Renderer.h"
#include <cmath>

using namespace Engine;

/**
 * Custom game class inheriting from Renderer
 */
class MyGame : public Renderer {
private:
    float m_playerX;
    float m_playerY;
    float m_playerSize;
    float m_playerSpeed;
    float m_angle;

public:
    MyGame()
        : m_playerX(400), m_playerY(300), m_playerSize(50)
        , m_playerSpeed(300.0f), m_angle(0) {
    }

    void OnUpdate(float deltaTime) override {
        // Movement
        float speed = m_playerSpeed * deltaTime;

        if (GetWindow()->IsKeyPressed('W')) m_playerY -= speed;
        if (GetWindow()->IsKeyPressed('S')) m_playerY += speed;
        if (GetWindow()->IsKeyPressed('A')) m_playerX -= speed;
        if (GetWindow()->IsKeyPressed('D')) m_playerX += speed;

        // Boundary collision
        int width = GetWindow()->GetWidth();
        int height = GetWindow()->GetHeight();

        if (m_playerX < 0) m_playerX = 0;
        if (m_playerY < 0) m_playerY = 0;
        if (m_playerX + m_playerSize > width) m_playerX = width - m_playerSize;
        if (m_playerY + m_playerSize > height) m_playerY = height - m_playerSize;

        // Animation
        m_angle += 2.0f * deltaTime;
        if (m_angle > 360.0f) m_angle -= 360.0f;
    }

    void OnRender() override {
        // Clear screen
        Clear(0.1f, 0.1f, 0.2f);

        // Draw rotating circle in corner
        glPushMatrix();
        glTranslatef(100, 100, 0);
        glRotatef(m_angle, 0, 0, 1);
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 0.5f, 0.2f);
        glVertex2f(0, 0);
        for (int i = 0; i <= 32; i++) {
            float angle = 2.0f * 3.14159f * i / 32;
            float x = 40 * cos(angle);
            float y = 40 * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
        glPopMatrix();

        // Draw colored rectangles
        for (int i = 0; i < 5; i++) {
            float hue = (i / 5.0f + m_angle / 360.0f);
            DrawRect(50 + i * 100, 500, 60, 40,
                sin(hue * 3.14f) * 0.5f + 0.5f,
                cos(hue * 3.14f * 1.5f) * 0.5f + 0.5f,
                sin(hue * 3.14f * 2.0f) * 0.5f + 0.5f);
        }

        // Draw player
        float hue = sin(m_angle * 0.1f) * 0.5f + 0.5f;
        DrawRect(m_playerX, m_playerY, m_playerSize, m_playerSize,
            hue, 0.5f, 1.0f - hue);

        // Draw UI
        DrawRect(5, 5, 790, 30, 0.2f, 0.2f, 0.3f);
        DrawRect(10, 10, 780, 20, 0.3f, 0.3f, 0.4f);
    }

    void OnKeyDown(int key) override {
        if (key == VK_ESCAPE) {
            // Close window on escape
            GetWindow()->Destroy();
        }
    }
};

/**
 * Entry point - completely platform independent!
 */
int main() {
    MyGame game;

    if (!game.Initialize(800, 600, "Cross-Platform Game Engine")) {
        return 1;
    }

    game.Run();
    game.Shutdown();

    return 0;
}