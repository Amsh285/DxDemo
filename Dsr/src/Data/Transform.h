#pragma once

namespace dsr
{
	namespace data
	{
		class Transform
		{
		public:
			DirectX::XMVECTOR Position;
			DirectX::XMVECTOR Scale;
			DirectX::XMVECTOR Rotation;
			DirectX::XMVECTOR Pivot;

			DirectX::XMVECTOR CalculatePivot() const { return DirectX::XMVectorAdd(Position, Pivot); }

			Transform();
		private:
		};
	}
}
