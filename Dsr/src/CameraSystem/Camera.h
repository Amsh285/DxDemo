#pragma once

#include "Data/Transform.h"

namespace dsr
{
	namespace camerasystem
	{
		class Camera
		{
		public:
			dsr::data::Transform Transform;
			float FovAngleY;
			float AspectRatio;
			float NearZ;
			float FarZ;

			DirectX::XMMATRIX GetProjectionMatrix() const;
			DirectX::XMMATRIX GetViewMatrix() const;

			Camera();
			Camera(const Camera& other) = delete;
			Camera& operator=(const Camera& other) = delete;
		private:
		};
	}
}
