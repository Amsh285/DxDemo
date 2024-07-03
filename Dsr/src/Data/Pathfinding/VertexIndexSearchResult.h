#pragma once

namespace dsr
{
	namespace data
	{
		namespace pathfinding
		{
			enum class VertexIndexSearchResultType
			{
				NoIntersection,
				CoTriangular,
				Concurrent,
				PathSearchRequired
			};

			class VertexIndexSearchResult
			{
			public:
				uint32_t GetStartIndex() const { return m_startIndex; }
				uint32_t GetFinishIndex() const { return m_finishIndex; }
				VertexIndexSearchResultType GetResultType() const { return m_ResultType; }

				VertexIndexSearchResult(
					const uint32_t startIndex,
					const uint32_t finishIndex,
					const VertexIndexSearchResultType resultType
				) : m_startIndex(startIndex), m_finishIndex(finishIndex), m_ResultType(resultType)
				{
				}

			private:
				uint32_t m_startIndex;
				uint32_t m_finishIndex;
				VertexIndexSearchResultType m_ResultType;
			};
		}
	}
}
