#include "pch.h"
#include "Chunk.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include "FastNoiseSIMD/FastNoiseSIMD.h"

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
    FastNoiseSIMD* noiseGen = FastNoiseSIMD::NewFastNoiseSIMD();

    noiseGen->SetFrequency(0.10);
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

    CreateMesh();
}

void Chunk::CreateMesh()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    count = 0;

    for (int x = 0; x < CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < CHUNK_SIZE; ++z)
            {
                Block& block = m_blocks.at(x * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + z);
                if (block.IsActive())
                {
                    count++;
                    modelMatrices.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)));
                    colors.push_back(glm::vec4(block.GetRed() / 255.0, block.GetGreen() / 255.0,
                                               block.GetBlue() / 255.0, block.GetAlpha() / 255.0));
                }
            }
        }
    }

    unsigned int matrixBuffer;
    glGenBuffers(1, &matrixBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, matrixBuffer);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), modelMatrices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), nullptr);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    unsigned int colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec4), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(6);

    glVertexAttribDivisor(6, 1);
}

void Chunk::Render(const Shader& shader)
{
    shader.Use();
    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, count);
}
