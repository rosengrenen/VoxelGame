#include "pch.h"
#include "ChunkManager.h"
#include "Timer.h"

ChunkManager::ChunkManager(const Camera& camera) : m_camera(camera)
{
    CreateChunk(0, 0, 0);
}

void ChunkManager::CreateChunk(int x, int y, int z)
{
    glm::ivec3 gridPos(x, y, z);
    auto chunk = std::make_shared<Chunk>(gridPos, m_camera);
    chunk.get()->Setup();
    chunk.get()->CreateMesh();

    auto c = m_chunks.find(glm::ivec3(x + 1, y, z));
    if (c != m_chunks.end())
    {
        c->second->SetChunkXMinus(chunk);
        c->second->CreateMesh();
        chunk->SetChunkXPlus(c->second);
    }
    c = m_chunks.find(glm::ivec3(x - 1, y, z));
    if (c != m_chunks.end())
    {
        c->second->SetChunkXPlus(chunk);
        c->second->CreateMesh();
        chunk->SetChunkXMinus(c->second);
    }
    c = m_chunks.find(glm::ivec3(x, y, z + 1));
    if (c != m_chunks.end())
    {
        c->second->SetChunkZMinus(chunk);
        c->second->CreateMesh();
        chunk->SetChunkZPlus(c->second);
    }
    c = m_chunks.find(glm::ivec3(x, y, z - 1));
    if (c != m_chunks.end())
    {
        c->second->SetChunkZPlus(chunk);
        c->second->CreateMesh();
        chunk->SetChunkZMinus(c->second);
    }

    m_chunks[gridPos] = chunk;
}

void ChunkManager::Update()
{
    Timer updateTimer;
    int chunkUpdates = 0;
    int maxPerFrame = 10;

    //std::sort(chunks.begin(), chunks.end(), Chunk::ClosestToCamera);

    for (auto chunkPtr : m_chunks)
    {
        auto chunk = chunkPtr.second.get();

        auto gridPos = chunk->getGridPos();
        gridPos *= Chunk::CHUNK_SIZE;
        gridPos += Chunk::CHUNK_SIZE / 2;

        glm::vec3 distanceVec = m_camera.position() - glm::vec3(gridPos);
        float distance = glm::length(distanceVec);

        if (distance > S_RENDER_DISTANCE)
        {
            // REMOVE
        }
        else
        {
            // UPDATE

            auto pos = chunk->getGridPos();
            pos *= Chunk::CHUNK_SIZE;
            pos += Chunk::CHUNK_SIZE / 2;

            auto pos1 = chunk->getGridPos();
            // CHECK FOR NEIGHBOURS
            if (m_chunks.find(chunk->getGridPos() + glm::ivec3(1, 0, 0)) == m_chunks.end())
            {
                if (glm::length(m_camera.position() - glm::vec3(pos + glm::ivec3(1 * Chunk::CHUNK_SIZE, 0, 0))) < S_RENDER_DISTANCE)
                {
                    chunkUpdates++;
                    CreateChunk(pos1.x + 1, pos1.y, pos1.z);
                    if (chunkUpdates >= maxPerFrame)
                    {
                        return;
                    }
                }
            }
            if (m_chunks.find(chunk->getGridPos() + glm::ivec3(-1, 0, 0)) == m_chunks.end())
            {
                if (glm::length(m_camera.position() - glm::vec3(pos + glm::ivec3(-1 * Chunk::CHUNK_SIZE, 0, 0))) < S_RENDER_DISTANCE)
                {
                    chunkUpdates++;
                    CreateChunk(pos1.x - 1, pos1.y, pos1.z);
                    if (chunkUpdates >= maxPerFrame)
                    {
                        return;
                    }
                }
            }
            if (m_chunks.find(chunk->getGridPos() + glm::ivec3(0, 0, 1)) == m_chunks.end())
            {
                if (glm::length(m_camera.position() - glm::vec3(pos + glm::ivec3(0, 1 * Chunk::CHUNK_SIZE, 0))) < S_RENDER_DISTANCE)
                {
                    chunkUpdates++;
                    CreateChunk(pos1.x, pos1.y, pos1.z + 1);
                    if (chunkUpdates >= maxPerFrame)
                    {
                        return;
                    }
                }
            }
            if (m_chunks.find(chunk->getGridPos() + glm::ivec3(0, 0, -1)) == m_chunks.end())
            {
                if (glm::length(m_camera.position() - glm::vec3(pos + glm::ivec3(0, -1 * Chunk::CHUNK_SIZE, 0))) < S_RENDER_DISTANCE)
                {
                    chunkUpdates++;
                    CreateChunk(pos1.x, pos1.y, pos1.z - 1);
                    if (chunkUpdates >= maxPerFrame)
                    {
                        return;
                    }
                }
            }
        }
    }
    //LOG_INFO("Update time: {} seconds", updateTimer.Elapsed());
}

void ChunkManager::Render(Shader& shader)
{
    int triangles = 0;
    for (const auto chunk : m_chunks)
    {
        chunk.second.get()->Render(shader);
        triangles += chunk.second.get()->count / 3;
    }
    //LOG_INFO("Triangles: {}", triangles);
}

void ChunkManager::RebuildAll()
{
    for (auto chunk : m_chunks)
    {
        chunk.second->CreateMesh();
    }
}
