#include "pch.h"
#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "Camera.h"
#include "Chunk.h"
#include "Shader.h"
#include "Timer.h"

Game::Game() : m_window(Window(800, 600, "Game"))
{
    m_frameTime = 1.0 / 10000;
}

void Game::Start()
{
    if (m_running)
    {
        return;
    }

    Run();
}

void Game::Update() {}

float* GenerateCameraMesh(const Camera& cam, float fov, float aspect, float nearPlane, float farPlane)
{
    float fovV = fov;
    float fovH = fovV * aspect;
    nearPlane /= glm::cos(glm::radians(fovV / 2));
    nearPlane /= glm::cos(glm::radians(fovH / 2));
    farPlane /= glm::cos(glm::radians(fovV / 2));
    farPlane /= glm::cos(glm::radians(fovH / 2));

    glm::vec3 topLeft;
    topLeft.x = glm::cos(glm::radians(cam.pitch() + fovV / 2.0f)) * glm::cos(glm::radians(cam.yaw() + fovH / 2.0f));
    topLeft.y = glm::sin(glm::radians(cam.pitch() + fovV / 2.0f));
    topLeft.z = glm::cos(glm::radians(cam.pitch() + fovV / 2.0f)) * glm::sin(glm::radians(cam.yaw() + fovH / 2.0f));
    topLeft = glm::normalize(topLeft);
    glm::vec3 bottomLeft;
    bottomLeft.x = glm::cos(glm::radians(cam.pitch() - fovV / 2.0f)) * glm::cos(glm::radians(cam.yaw() + fovH / 2.0f));
    bottomLeft.y = glm::sin(glm::radians(cam.pitch() - fovV / 2.0f));
    bottomLeft.z = glm::cos(glm::radians(cam.pitch() - fovV / 2.0f)) * glm::sin(glm::radians(cam.yaw() + fovH / 2.0f));
    bottomLeft = glm::normalize(bottomLeft);
    glm::vec3 topRight;
    topRight.x = glm::cos(glm::radians(cam.pitch() + fovV / 2.0f)) * glm::cos(glm::radians(cam.yaw() - fovH / 2.0f));
    topRight.y = glm::sin(glm::radians(cam.pitch() + fovV / 2.0f));
    topRight.z = glm::cos(glm::radians(cam.pitch() + fovV / 2.0f)) * glm::sin(glm::radians(cam.yaw() - fovH / 2.0f));
    topRight = glm::normalize(topRight);
    glm::vec3 bottomRight;
    bottomRight.x = glm::cos(glm::radians(cam.pitch() - fovV / 2.0f)) * glm::cos(glm::radians(cam.yaw() - fovH / 2.0f));
    bottomRight.y = glm::sin(glm::radians(cam.pitch() - fovV / 2.0f));
    bottomRight.z = glm::cos(glm::radians(cam.pitch() - fovV / 2.0f)) * glm::sin(glm::radians(cam.yaw() - fovH / 2.0f));
    bottomRight = glm::normalize(bottomRight);

    float* mesh = new float[72];
    // Top left
    mesh[0] = cam.position().x;
    mesh[1] = cam.position().y;
    mesh[2] = cam.position().z;
    mesh[3] = cam.position().x + topLeft.x * farPlane;
    mesh[4] = cam.position().y + topLeft.y * farPlane;
    mesh[5] = cam.position().z + topLeft.z * farPlane;

    // Bottom left
    mesh[6] = cam.position().x;
    mesh[7] = cam.position().y;
    mesh[8] = cam.position().z;
    mesh[9] = cam.position().x + bottomLeft.x * farPlane;
    mesh[10] = cam.position().y + bottomLeft.y * farPlane;
    mesh[11] = cam.position().z + bottomLeft.z * farPlane;

    // Top right
    mesh[12] = cam.position().x;
    mesh[13] = cam.position().y;
    mesh[14] = cam.position().z;
    mesh[15] = cam.position().x + topRight.x * farPlane;
    mesh[16] = cam.position().y + topRight.y * farPlane;
    mesh[17] = cam.position().z + topRight.z * farPlane;

    // Bottom right
    mesh[18] = cam.position().x;
    mesh[19] = cam.position().y;
    mesh[20] = cam.position().z;
    mesh[21] = cam.position().x + bottomRight.x * farPlane;
    mesh[22] = cam.position().y + bottomRight.y * farPlane;
    mesh[23] = cam.position().z + bottomRight.z * farPlane;

    // Far top
    mesh[24] = cam.position().x + topLeft.x * farPlane;
    mesh[25] = cam.position().y + topLeft.y * farPlane;
    mesh[26] = cam.position().z + topLeft.z * farPlane;
    mesh[27] = cam.position().x + topRight.x * farPlane;
    mesh[28] = cam.position().y + topRight.y * farPlane;
    mesh[29] = cam.position().z + topRight.z * farPlane;

    // Far right
    mesh[30] = cam.position().x + topRight.x * farPlane;
    mesh[31] = cam.position().y + topRight.y * farPlane;
    mesh[32] = cam.position().z + topRight.z * farPlane;
    mesh[33] = cam.position().x + bottomRight.x * farPlane;
    mesh[34] = cam.position().y + bottomRight.y * farPlane;
    mesh[35] = cam.position().z + bottomRight.z * farPlane;

    // Far bottom
    mesh[36] = cam.position().x + bottomRight.x * farPlane;
    mesh[37] = cam.position().y + bottomRight.y * farPlane;
    mesh[38] = cam.position().z + bottomRight.z * farPlane;
    mesh[39] = cam.position().x + bottomLeft.x * farPlane;
    mesh[40] = cam.position().y + bottomLeft.y * farPlane;
    mesh[41] = cam.position().z + bottomLeft.z * farPlane;

    // Far left
    mesh[42] = cam.position().x + bottomLeft.x * farPlane;
    mesh[43] = cam.position().y + bottomLeft.y * farPlane;
    mesh[44] = cam.position().z + bottomLeft.z * farPlane;
    mesh[45] = cam.position().x + topLeft.x * farPlane;
    mesh[46] = cam.position().y + topLeft.y * farPlane;
    mesh[47] = cam.position().z + topLeft.z * farPlane;

    // Near top
    mesh[48] = cam.position().x + topLeft.x * nearPlane;
    mesh[49] = cam.position().y + topLeft.y * nearPlane;
    mesh[50] = cam.position().z + topLeft.z * nearPlane;
    mesh[51] = cam.position().x + topRight.x * nearPlane;
    mesh[52] = cam.position().y + topRight.y * nearPlane;
    mesh[53] = cam.position().z + topRight.z * nearPlane;

    // Near right
    mesh[54] = cam.position().x + topRight.x * nearPlane;
    mesh[55] = cam.position().y + topRight.y * nearPlane;
    mesh[56] = cam.position().z + topRight.z * nearPlane;
    mesh[57] = cam.position().x + bottomRight.x * nearPlane;
    mesh[58] = cam.position().y + bottomRight.y * nearPlane;
    mesh[59] = cam.position().z + bottomRight.z * nearPlane;

    // Near bottom
    mesh[60] = cam.position().x + bottomRight.x * nearPlane;
    mesh[61] = cam.position().y + bottomRight.y * nearPlane;
    mesh[62] = cam.position().z + bottomRight.z * nearPlane;
    mesh[63] = cam.position().x + bottomLeft.x * nearPlane;
    mesh[64] = cam.position().y + bottomLeft.y * nearPlane;
    mesh[65] = cam.position().z + bottomLeft.z * nearPlane;

    // Near left
    mesh[66] = cam.position().x + bottomLeft.x * nearPlane;
    mesh[67] = cam.position().y + bottomLeft.y * nearPlane;
    mesh[68] = cam.position().z + bottomLeft.z * nearPlane;
    mesh[69] = cam.position().x + topLeft.x * nearPlane;
    mesh[70] = cam.position().y + topLeft.y * nearPlane;
    mesh[71] = cam.position().z + topLeft.z * nearPlane;

    return mesh;
}

void Game::Run()
{
    m_running = true;

    m_window.SetCursorVisible(false);

    constexpr double updateTime = 1.0 / 100.0;
    double fov = 45.0;
    double nearPlane = 0.1;
    double farPlane = 1000.0;

    constexpr float vertices[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f,
    };

    glm::mat4 proj = glm::perspective(glm::radians(fov),
                                      static_cast<double>(m_window.GetWidth()) / m_window.GetHeight(), nearPlane,
                                      farPlane);
    glm::mat4 model(1.0f);

    Camera camera(0, 0, 1);
    Camera camera1(0, 0, 1);

    Shader shader("res/shaders/triangle.vert.glsl", "res/shaders/triangle.frag.glsl");
    shader.SetMat4("model", model);
    shader.SetMat4("projection", proj);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    float* lines = GenerateCameraMesh(camera, fov, static_cast<double>(m_window.GetWidth()) / m_window.GetHeight(),
                                      nearPlane, farPlane);

    unsigned int lVao;
    glGenVertexArrays(1, &lVao);
    glBindVertexArray(lVao);

    unsigned int lVbo;
    glGenBuffers(1, &lVbo);
    glBindBuffer(GL_ARRAY_BUFFER, lVbo);
    glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), lines, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    Shader chunkShader("res/shaders/chunk.vert.glsl", "res/shaders/chunk.frag.glsl");
    chunkShader.SetMat4("projection", proj);

    Chunk chunk;
    chunk.Setup();

    Timer camSwapTimer;

    Timer timer;
    Timer frameTimer;
    double unprocessedUpdateTime = 0;
    double unprocessedRenderTime = 0;
    int frames = 0;

    glm::vec2 mousePos = m_window.GetMousePos();

    while (m_running)
    {
        double dt = timer.Reset();
        unprocessedUpdateTime += dt;
        unprocessedRenderTime += dt;
        int updatesThisLoop = 0;

        double dx = 0.0, dy = 0.0;
        while (unprocessedUpdateTime > updateTime)
        {
            updatesThisLoop++;
            unprocessedUpdateTime -= updateTime;

            if (updatesThisLoop == 1)
            {
                m_window.PollEvents();
                glm::vec2 currentMousePos = m_window.GetMousePos();
                dx = currentMousePos.x - mousePos.x;
                dy = currentMousePos.y - mousePos.y;
                mousePos = currentMousePos;
            }

            if (dx != 0 || dy != 0)
            {
                camera.MouseMovement(dx, dy);
            }

            if (m_window.KeyDown(GLFW_KEY_SPACE))
            {
                camera.MoveUp(updateTime);
            }
            else if (m_window.KeyDown(GLFW_KEY_LEFT_SHIFT))
            {
                camera.MoveDown(updateTime);
            }
            if (m_window.KeyDown(GLFW_KEY_D))
            {
                camera.MoveRight(updateTime);
            }
            else if (m_window.KeyDown(GLFW_KEY_A))
            {
                camera.MoveLeft(updateTime);
            }
            if (m_window.KeyDown(GLFW_KEY_W))
            {
                camera.MoveForward(updateTime);
            }
            else if (m_window.KeyDown(GLFW_KEY_S))
            {
                camera.MoveBackwards(updateTime);
            }

            bool update = false;
            double perSecondChange = updateTime;
            double planeChange = updateTime * 10;
            double fovChange = updateTime * 10;
            if (m_window.KeyDown(GLFW_KEY_KP_1))
            {
                update = true;
                farPlane -= planeChange;
                if (farPlane < nearPlane)
                {
                    farPlane = nearPlane + 0.01;
                }
            }
            else if (m_window.KeyDown(GLFW_KEY_KP_4))
            {
                update = true;
                farPlane += planeChange;
                if (farPlane > 2000.0)
                {
                    farPlane = 2000.0;
                }
            }

            if (m_window.KeyDown(GLFW_KEY_KP_2))
            {
                update = true;
                nearPlane -= planeChange;
                if (nearPlane < 0.0001)
                {
                    nearPlane = 0.0001;
                }
            }
            else if (m_window.KeyDown(GLFW_KEY_KP_5))
            {
                update = true;
                nearPlane += planeChange;
                if (nearPlane >= farPlane)
                {
                    nearPlane = farPlane - 0.01;
                }
            }

            if (m_window.KeyDown(GLFW_KEY_KP_3))
            {
                update = true;
                fov -= fovChange;
                if (fov < 10.0)
                {
                    fov = 10.0;
                }
            }
            else if (m_window.KeyDown(GLFW_KEY_KP_6))
            {
                update = true;
                fov += fovChange;
                if (fov > 170.0)
                {
                    fov = 170.0;
                }
            }
            if (update)
            {
                shader.SetMat4("projection",
                               glm::perspective(glm::radians(fov),
                                                static_cast<double>(m_window.GetWidth()) / m_window.GetHeight(),
                                                nearPlane, farPlane));
                chunkShader.SetMat4("projection",
                    glm::perspective(glm::radians(fov),
                        static_cast<double>(m_window.GetWidth()) / m_window.GetHeight(),
                        nearPlane, farPlane));
            }

            if (m_window.KeyDown(GLFW_KEY_1) && camSwapTimer.Elapsed() > 0.1)
            {
                lines = GenerateCameraMesh(camera, fov, static_cast<double>(m_window.GetWidth()) / m_window.GetHeight(),
                                           nearPlane, farPlane);

                camSwapTimer.Reset();
                Camera tmp = camera;
                camera = camera1;
                camera1 = tmp;

                glBindVertexArray(lVao);
                glBindBuffer(GL_ARRAY_BUFFER, lVbo);
                glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), lines, GL_DYNAMIC_DRAW);
            }

            Update();
        }
        if (updatesThisLoop == 0)
        {
            //std::this_thread::sleep_for(std::chrono::microseconds(5000));
        }
        if (frameTimer.Elapsed() > 1.0)
        {
            double time = frameTimer.Reset();
            LOG_INFO("FPS: {}", static_cast<double>(frames) / time);
            frames = 0;
        }
        if (unprocessedRenderTime > m_frameTime)
        {
            frames++;

            m_window.Clear();

            chunkShader.SetMat4("view", camera.ViewMatrix());

            shader.Use();
            shader.SetMat4("view", camera.ViewMatrix());
            shader.SetVec3("color", 1.0f, 0.0f, 0.0f);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            chunk.Render(chunkShader);

            shader.SetVec3("color", 0.0f, 1.0f, 0.0f);
            glBindVertexArray(lVao);
            glDrawArrays(GL_LINES, 0, 72);

            m_window.Display();
            unprocessedRenderTime -= m_frameTime;
        }
        else
        {
            //std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }
}
