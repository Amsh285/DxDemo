#pragma once

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			class VertexIndexSearchResult
			{
			public:

				// Check if start and finish points are in the same triangle
				bool IsCoTriangular() const { return m_coTriangular; }

				// Check if nearest Triangle Vertices are the same
				bool IsConcurrent() const { return m_startIndex == m_finishIndex; }

				VertexIndexSearchResult(
					const uint32_t startIndex,
					const uint32_t finishIndex,
					const bool coTriangular
				) : m_startIndex(startIndex), m_finishIndex(finishIndex), m_coTriangular(coTriangular)
				{
				}

			private:
				uint32_t m_startIndex;
				uint32_t m_finishIndex;

				bool m_coTriangular;
			};
		}
	}
}
