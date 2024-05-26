#pragma once

#include "DirectX/Direct3dDevice.h"


#include "EngineSubSystems/EntityComponentSystem/EngineContext.h"
#include "EngineSubSystems/EntityComponentSystem/EnginePrepareRendererContext.h"
#include "EngineSubSystems/EntityComponentSystem/EngineStartupContext.h"

#include "EngineSubSystems/EntityComponentSystem/Components/ShaderProgramComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/TransformComponent.h"
#include "EngineSubSystems/EntityComponentSystem/Components/WireframeMeshComponent.h"

#include "EngineSubSystems/EntityComponentSystem/EcsSystems/Renderers/RendererSystem.h"

#include "EngineSubSystems/SceneSystem/Camera.h"

namespace dsr
{
	namespace ecs
	{
		class WireframeRendererSystem final : public RendererSystem
		{
		public:
			virtual std::vector<std::type_index> GetRequiredComponents() const override;

			WireframeRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device);

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

			template<>
			void ApplyConstantBuffers<ID3D11PixelShader>()
			{
				std::vector<ID3D11Buffer*> psConstantBuffers;
				for (auto& pair : m_psConstantBuffers)
					psConstantBuffers.push_back(pair.second.GetBufferPtr().get());

				m_device->UseConstantBuffers<ID3D11PixelShader>(0, psConstantBuffers.size(), psConstantBuffers.data());
			}

			void BindVertexBuffer(const dsr::directX::Direct3dVertexBufferf& vertexBuffer);

			std::shared_ptr<directX::Direct3dShaderProgram> m_shaderProgram;

			std::shared_ptr<ID3D11RasterizerState> m_rasterizerState;
			std::shared_ptr<TransformComponent> m_cameraTransform;
		};
	}
}
