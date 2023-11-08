#include "dsrpch.h"
#include "Direct3dRenderUoW.h"

namespace dsr
{
	namespace directX
	{
		namespace rendering
		{
			Direct3dRenderUoW::Direct3dRenderUoW(const Direct3dShaderProgram& shaderProgram)
				: m_shaderProgram(shaderProgram)
			{
			}
		}
	}
}
