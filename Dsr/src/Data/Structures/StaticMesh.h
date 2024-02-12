#pragma once

namespace dsr
{
	namespace data
	{
		template<class TVertex>
		class StaticMesh
		{
		public:
			std::vector<TVertex> VertexBuffer;
			std::vector<uint32_t> IndexBuffer;
		private:
		};
	}
}
