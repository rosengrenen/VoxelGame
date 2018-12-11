#pragma once

#include "FastNoise/FastNoise.h"

#include "Block.h"
#include "Camera.h"
#include "Shader.h"

class Chunk
{
public:
    /* Public methods */
    Chunk(const glm::ivec3& gridPos, const Camera& camera);

    void Setup();
    void CreateMesh();
    void Render(Shader& shader);

    bool IsActive(int x, int y, int z);

    bool operator<(const Chunk& other);
    const glm::ivec3& getGridPos() const;

    static bool ClosestToCamera(std::shared_ptr<Chunk>left, std::shared_ptr<Chunk> right);

    void SetChunkXPlus(std::shared_ptr<Chunk> chunk);
    void SetChunkXMinus(std::shared_ptr<Chunk> chunk);
    void SetChunkZPlus(std::shared_ptr<Chunk> chunk);
    void SetChunkZMinus(std::shared_ptr<Chunk> chunk);

public:
    /* Public members */
    static constexpr int CHUNK_SIZE = 32;
    static constexpr int CHUNK_SIZE_SQUARED = CHUNK_SIZE * CHUNK_SIZE;
    static constexpr int CHUNK_SIZE_CUBED = CHUNK_SIZE_SQUARED * CHUNK_SIZE;

private:
    /* Private members */
    std::vector<Block> m_blocks;
    glm::ivec3 m_gridPos;
    const Camera& m_camera;

    // Flags
    bool m_setup;

    // Temp
    unsigned int vao;
    unsigned int vbo;
public:
    unsigned int count;
private:
    std::vector<glm::mat4> modelMatrices;
    std::vector<glm::vec4> colors;
    static FastNoise m_noise;

    // Neighbours
    std::shared_ptr<Chunk> m_xPlus;
    std::shared_ptr<Chunk> m_xMinus;
    std::shared_ptr<Chunk> m_zPlus;
    std::shared_ptr<Chunk> m_zMinus;
};

