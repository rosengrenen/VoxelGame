#include "pch.h"
#include "Chunk.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include "FastNoiseSIMD/FastNoiseSIMD.h"
#include "Timer.h"

static constexpr float cubeVertices[] = {
    // positions         // normals         // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, // 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 0.0f, 1.0f
};

Chunk::Chunk()
{
    m_blocks.reserve(CHUNK_SIZE_CUBED);
    m_setup = false;
}

void Chunk::Setup()
{
    LOG_INFO("Chunk size: {}", CHUNK_SIZE);
    Timer masterTimer;
    FastNoiseSIMD* noiseGen = FastNoiseSIMD::NewFastNoiseSIMD();

    noiseGen->SetFrequency(0.1);
    float* noise = noiseGen->GetPerlinSet(0, 0, 0, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);
    int index = 0;

    m_blocks.resize(CHUNK_SIZE_CUBED);

    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                if (noise[index] > 0.0f)
                {
                    unsigned char red = 0;
                    unsigned char green = 0;
                    unsigned char blue = 0;

                    double val = static_cast<double>(y) / CHUNK_SIZE;
                    if (val < 0.33)
                    {
                        red = 119;
                        green = 244;
                        blue = 66;
                    }
                    else if (val < 0.66)
                    {
                        red = 84;
                        green = 107;
                        blue = 107;
                    }
                    else
                    {
                        red = 206;
                        green = 239;
                        blue = 239;
                    }

                    m_blocks.at(x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z) = Block(red, green, blue, 0);
                }
                index++;
            }
        }
    }
    LOG_INFO("Total time: {} seconds", masterTimer.Elapsed());

    CreateMesh();
}

void AddVertex(std::vector<float>& data, int x, int y, int z, int nx, int ny, int nz, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    data.push_back(x);
    data.push_back(y);
    data.push_back(z);
    data.push_back(nx);
    data.push_back(ny);
    data.push_back(nz);
    data.push_back(r / 255.0);
    data.push_back(g / 255.0);
    data.push_back(b / 255.0);
    data.push_back(a / 255.0);
}

void Chunk::CreateMesh()
{
    std::vector<float> data;
    data.reserve(CHUNK_SIZE_CUBED / 2 * 6 * 6 * 10 * 0.1);

    Timer meshGenerationTimer;
    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                Block& block = m_blocks[x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z];
                if (block.IsActive())
                {
                    bool xPlus = x == CHUNK_SIZE - 1 || (x < CHUNK_SIZE - 1 && !m_blocks[(x + 1) * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z].
                            IsActive());
                    bool xMinus = x == 0 || (x != 0 && !m_blocks[(x - 1) * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z].IsActive());
                    bool yPlus = y == CHUNK_SIZE - 1 || (y < CHUNK_SIZE - 1 && !m_blocks[x * CHUNK_SIZE_SQUARED + (y + 1) * CHUNK_SIZE + z].
                            IsActive());
                    bool yMinus = y == 0 || (y != 0 && !m_blocks[x * CHUNK_SIZE_SQUARED + (y - 1) * CHUNK_SIZE + z].IsActive());
                    bool zPlus = z == CHUNK_SIZE - 1 || (z < CHUNK_SIZE - 1 && !m_blocks[x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z + 1].
                            IsActive());
                    bool zMinus = z == 0 || (z != 0 && !m_blocks[x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z - 1].IsActive());
                    if (xPlus)
                    {
                        count += 6;
                        AddVertex(data,
                                  0.5f + x, 0.5f + y, 0.5f + z,
                                  1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, -0.5f + y, 0.5f + z,
                                  1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, -0.5f + y, -0.5f + z,
                                  1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, -0.5f + y, -0.5f + z,
                                  1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, 0.5f + y, -0.5f + z,
                                  1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, 0.5f + y, 0.5f + z,
                                  1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                    }
                    if (xMinus)
                    {
                        count += 6;
                        AddVertex(data,
                                  -0.5f + x, 0.5f + y, 0.5f + z,
                                  -1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, 0.5f + y, -0.5f + z,
                                  -1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, -0.5f + y, -0.5f + z,
                                  -1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, -0.5f + y, -0.5f + z,
                                  -1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, -0.5f + y, 0.5f + z,
                                  -1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, 0.5f + y, 0.5f + z,
                                  -1.0f, 0.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                    }
                    if (yPlus)
                    {
                        count += 6;
                        AddVertex(data,
                                  0.5f + x, 0.5f + y, 0.5f + z,
                                  0.0f, 1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, 0.5f + y, -0.5f + z,
                                  0.0f, 1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, 0.5f + y, -0.5f + z,
                                  0.0f, 1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, 0.5f + y, -0.5f + z,
                                  0.0f, 1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, 0.5f + y, 0.5f + z,
                                  0.0f, 1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, 0.5f + y, 0.5f + z,
                                  0.0f, 1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                    }
                    if (yMinus)
                    {
                        count += 6;
                        AddVertex(data,
                                  0.5f + x, -0.5f + y, 0.5f + z,
                                  0.0f, -1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, -0.5f + y, 0.5f + z,
                                  0.0f, -1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, -0.5f + y, -0.5f + z,
                                  0.0f, -1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  -0.5f + x, -0.5f + y, -0.5f + z,
                                  0.0f, -1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, -0.5f + y, -0.5f + z,
                                  0.0f, -1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                                  0.5f + x, -0.5f + y, 0.5f + z,
                                  0.0f, -1.0f, 0.0f,
                                  block.GetRed(), block.GetGreen(),
                                  block.GetBlue(), block.GetAlpha()
                        );
                    }
                    if (zPlus)
                    {
                        count += 6;
                        AddVertex(data,
                            0.5f + x, 0.5f + y, 0.5f + z,
                            0.0f, 0.0f, 1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            -0.5f + x, 0.5f + y, 0.5f + z,
                            0.0f, 0.0f, 1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            -0.5f + x, -0.5f + y, 0.5f + z,
                            0.0f, 0.0f, 1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            -0.5f + x, -0.5f + y, 0.5f + z,
                            0.0f, 0.0f, 1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            0.5f + x, -0.5f + y, 0.5f + z,
                            0.0f, 0.0f, 1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            0.5f + x, 0.5f + y, 0.5f + z,
                            0.0f, 0.0f, 1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                    }
                    if (zMinus)
                    {
                        count += 6;
                        AddVertex(data,
                            0.5f + x, 0.5f + y, -0.5f + z,
                            0.0f, 0.0f, -1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            0.5f + x, -0.5f + y, -0.5f + z,
                            0.0f, 0.0f, -1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            -0.5f + x, -0.5f + y, -0.5f + z,
                            0.0f, 0.0f, -1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            -0.5f + x, -0.5f + y, -0.5f + z,
                            0.0f, 0.0f, -1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            -0.5f + x, 0.5f + y, -0.5f + z,
                            0.0f, 0.0f, -1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                        AddVertex(data,
                            0.5f + x, 0.5f + y, -0.5f + z,
                            0.0f, 0.0f, -1.0f,
                            block.GetRed(), block.GetGreen(),
                            block.GetBlue(), block.GetAlpha()
                        );
                    }
                }
            }
        }
    }
    Timer dataToGpuTimer;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    LOG_INFO("Mesh generation time: {} seconds", meshGenerationTimer.Elapsed());
}

void Chunk::Render(Shader& shader)
{
    shader.Use();
    shader.SetMat4("model", glm::mat4(1.0f));
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}
