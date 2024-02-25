#pragma once

namespace dsr
{
    struct XMVectorHasher
    {
        size_t operator()(const DirectX::XMVECTOR& vec) const
        {
            using namespace DirectX;

            // Extract components and scale them to integers for hashing
            XMVECTOR scaledVec = XMVectorScale(vec, 1000.0f); // Adjust scale factor as needed

            // Combine hash values of individual components
            size_t hash = 0;
            hash_combine(hash, std::hash<int>()(XMVectorGetIntX(scaledVec)));
            hash_combine(hash, std::hash<int>()(XMVectorGetIntY(scaledVec)));
            hash_combine(hash, std::hash<int>()(XMVectorGetIntZ(scaledVec)));
            hash_combine(hash, std::hash<int>()(XMVectorGetIntW(scaledVec)));

            return hash;
        }

        // Utility function for combining hash values
        static void hash_combine(size_t& seed, size_t hash)
        {
            seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };

    template<float epsilon>
	struct XMVectorEqualComparer
	{
		bool operator()(const DirectX::XMVECTOR& lhs, const DirectX::XMVECTOR& rhs) const
        {
            using namespace DirectX;

            bool consideredEqual = XMVector4NearEqual(lhs, rhs, XMVectorReplicate(epsilon));
            return consideredEqual;
        }
	};
}
