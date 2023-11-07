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

			DirectX::XMVECTOR CalculatePivotPoint() const { return DirectX::XMVectorAdd(Position, Pivot); }
			DirectX::XMMATRIX CalculateModelMatrix() const;

			Transform();
		private:
		};
	}
}
