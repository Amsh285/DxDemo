#pragma once

#include "DirectX/Direct3dDevice.h"
#include "DirectX/Direct3dDeviceBufferExtensions.h"
#include "DirectX/Shader/Direct3dShaderProgram.h"

#include "EngineSubSystems/EntityComponentSystem/Components/LineListComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/ShaderProgramComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"

#include "EngineSubSystems/EntityComponentSystem/EcsSystems/Renderers/RendererSystem.h"

namespace dsr
{
	namespace ecs
	{
		class LineListRendererSystem : public RendererSystem
		{
		public:
			virtual std::vector<std::type_index> GetRequiredComponents() const override;

			LineListRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device);

			void Startup(const EngineStartupContext& context);
			void PrepareRendererUpdate(const EnginePrepareRendererContext& context);
			void Update(const EngineContext& context);
		private:
			template<class TShader>
			void ApplyConstantBuffers();

			template<>
			void ApplyConstantBuffers<ID3D11VertexShader>()
			{
				std::vector<ID3D11Buffer*> vsConstantBuffers;
				for (auto& pair : m_vsConstantBuffers)
					vsConstantBuffers.push_back(pair.second.GetBufferPtr().get());

				m_device->UseConstantBuffers<ID3D11VertexShader>(0, vsConstantBuffers.size(), vsConstantBuffers.data());
			}

			/*template<>
			void ApplyConstantBuffers<ID3D11PixelShader>()
			{
				std::vector<ID3D11Buffer*> psConstantBuffers;
				for (auto& pair : m_psConstantBuffers)
					psConstantBuffers.push_back(pair.second.GetBufferPtr().get());

				m_device->UseConstantBuffers<ID3D11PixelShader>(0, psConstantBuffers.size(), psConstantBuffers.data());
			}*/

			std::shared_ptr<directX::Direct3dDevice> m_device;

			std::map<size_t, directX::Direct3dBuffer> m_vsConstantBuffers;
			std::shared_ptr<directX::Direct3dShaderProgram> m_shaderProgram;
		};
	}
}
