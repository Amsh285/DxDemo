#pragma once

namespace dsr
{
	namespace shader
	{
		struct PerObject
		{
			DirectX::XMMATRIX Model;
			DirectX::XMMATRIX Normal;
		};
	}
}
