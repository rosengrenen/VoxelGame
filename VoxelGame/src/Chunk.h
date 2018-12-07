#pragma once

#include "Block.h"
#include "Shader.h"

class Chunk
{
public:
    /* Public methods */
    Chunk();

    void Setup();
    void CreateMesh();
    void Render(Shader& shader);

public:
    /* Public members */
    static constexpr int CHUNK_SIZE = 128;
    static constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
    static constexpr int CHUNK_SIZE_CUBED = CHUNK_SIZE_SQUARED * CHUNK_SIZE;

private:
    /* Private members */
    std::vector<Block> m_blocks;

    // Flags
    bool m_setup;

    // Temp
    unsigned int vao;
    unsigned int count;
    std::vector<glm::mat4> modelMatrices;
    std::vector<glm::vec4> colors;
};

