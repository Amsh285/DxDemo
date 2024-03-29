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

			DirectX::XMMATRIX CalculateModelMatrix() const;
			DirectX::XMMATRIX CalculateNormalMatrix() const;

			Transform();
		private:
		};
	}
}
