#pragma once

#include "DirectX/Shader/Direct3dShaderProgram.h"

#include "EngineSubSystems/EntityComponentSystem/Component.h"

namespace dsr
{
	namespace ecs
	{
		class ShaderProgramComponent : public Component
		{
		public:
			std::shared_ptr<directX::Direct3dShaderProgram> GetShaderProgram() const { return m_shaderProgram; }
			void SetShaderProgram(const std::shared_ptr<directX::Direct3dShaderProgram>& shaderProgram) { m_shaderProgram = shaderProgram; }
		private:
			std::shared_ptr<directX::Direct3dShaderProgram> m_shaderProgram;
		};
	}
}
