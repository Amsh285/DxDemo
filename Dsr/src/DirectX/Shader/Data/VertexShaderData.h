#pragma once

//Todo: fix this namespace mess
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
