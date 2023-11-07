#pragma once

#include "Data/Transform.h"
#include "DirectX/Direct3dVertexBufferf.h"
#include "DirectX/Direct3dShaderProgram.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			class Direct3dRenderUoW
			{
			public:
				Direct3dVertexBufferf GetBuffer() const { return m_buffer; }
				Direct3dShaderProgram GetProgram() const { return m_shaderProgram; }

				// https://github.com/microsoft/DirectXTK/wiki/Multistream-rendering-and-instancing
				// can be used for instanced rendering. For now just take the first transform.
				std::vector<dsr::data::Transform> Transforms;

				Direct3dRenderUoW(const Direct3dVertexBufferf& buffer, const Direct3dShaderProgram& shaderProgram);
			private:
				Direct3dVertexBufferf m_buffer;

				// keep it simple for now
				Direct3dShaderProgram m_shaderProgram;
			};
		}
	}
}
