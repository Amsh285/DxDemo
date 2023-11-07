#pragma once

namespace dsr
{
	namespace data
	{
		class Transform
		{
		public:
			DirectX::XMFLOAT3 GetPosition() const { return m_position; }
			DirectX::XMFLOAT3 GetScale() const { return m_scale; }
			DirectX::XMFLOAT3 GetRotation() const { return m_rotation; }
			void SetPosition(const DirectX::XMFLOAT3& position) { m_position = position; }
			void SetScale(const DirectX::XMFLOAT3& scale) { m_scale = scale; }
			void SetRotation(const DirectX::XMFLOAT3& rotation) { m_rotation = rotation; }

			Transform();
		private:
			DirectX::XMFLOAT3 m_position;
			DirectX::XMFLOAT3 m_scale;
			DirectX::XMFLOAT3 m_rotation;
		};
	}
}
