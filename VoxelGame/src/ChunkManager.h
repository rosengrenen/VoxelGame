#pragma once

#include "Camera.h"
#include "Chunk.h"

namespace std
{
    template<>
    struct hash<glm::ivec3>
    {
        std::size_t operator()(const glm::ivec3& k) const
        {
            using std::size_t;
            using std::hash;
            using std::string;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return ((hash<int>()(k.x)
                        ^ (hash<int>()(k.y) << 1)) >> 1)
                    ^ (hash<int>()(k.z) << 1);
        }
    };
}

class ChunkManager
{
public:
    /* Public methods */
    ChunkManager(const Camera& camera);

    void CreateChunk(int x, int y, int z);

    void Update();

    void Render(Shader& shader);

    void RebuildAll();

private:
    /* Private members */
    std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> m_chunks;
    const Camera& m_camera;

    constexpr static int S_RENDER_DISTANCE = 1000;
};
