#include "dsrpch.h"
#include "WireframeRendererSystem.h"



namespace dsr
{
	namespace ecs
	{
		std::vector<std::type_index> WireframeRendererSystem::GetRequiredComponents() const
		{
			return { std::type_index(typeid(TransformComponent)) ,std::type_index(typeid(WireframeMeshComponent)) };
		}

		WireframeRendererSystem::WireframeRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device)
			: RendererSystem(typeid(WireframeRendererSystem), device)
		{
			OnStart = std::bind(&WireframeRendererSystem::Startup, this, std::placeholders::_1);
			OnPrepareRendererUpdate = std::bind(&WireframeRendererSystem::PrepareRendererUpdate, this, std::placeholders::_1);
			OnUpdate = std::bind(&WireframeRendererSystem::Update, this, std::placeholders::_1);
		}

		void WireframeRendererSystem::Startup(const EngineStartupContext& context)
		{
			using namespace DirectX;

			D3D11_RASTERIZER_DESC rasterizerDesc;
			ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
			rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthClipEnable = true;
			//rasterizerDesc.FrontCounterClockwise = false;

			std::variant<ID3D11RasterizerState*, dsr_error> createRasterizerStateResult = m_device->CreateRasterizerState(&rasterizerDesc);

			if (!std::holds_alternative<dsr_error>(createRasterizerStateResult))
			{
				ID3D11RasterizerState* rasterizerState = std::get<ID3D11RasterizerState*>(createRasterizerStateResult);
				m_rasterizerState = std::shared_ptr<ID3D11RasterizerState>(rasterizerState, [](ID3D11RasterizerState* ptr) {SafeRelease(ptr); });
			}
			else
			{
				// Todo: Log warning
			}

			std::vector<Entity> defaultShaderProgramEntities = context.FindEntitiesByTag("DefaultShaderProgram");

			if (!defaultShaderProgramEntities.empty())
			{
				std::shared_ptr<ShaderProgramComponent> programComponent = context.GetComponentFrom<ShaderProgramComponent>(defaultShaderProgramEntities[0]);
				m_shaderProgram = programComponent->GetShaderProgram();
			}
			else
			{
				//me too
				//get a logger already...
			}
		}

		void WireframeRendererSystem::PrepareRendererUpdate(const EnginePrepareRendererContext& context)
		{
			using namespace DirectX;

			using namespace dsr::scene;

			m_device->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			if (m_rasterizerState)
				m_device->SetRasterizerState(m_rasterizerState.get());

			SetupCamera();

			if (m_shaderProgram)
			{
				m_device->SetInputLayout(m_shaderProgram->VertexShaderInputLayout.get());
				m_device->UseShader(m_shaderProgram->VertexShader->GetShaderPtr().get(), nullptr, 0);
				m_device->UseShader(m_shaderProgram->PixelShader->GetShaderPtr().get(), nullptr, 0);
			}
		}

		void WireframeRendererSystem::Update(const EngineContext& context)
		{
			using namespace dsr::directX;
			using namespace dsr::directX::rendering;

			std::shared_ptr<TransformComponent> transform = context.GetComponent<TransformComponent>();
			std::shared_ptr<WireframeMeshComponent> wireframeMesh = context.GetComponent<WireframeMeshComponent>();

			RenderTransform renderTransform;

			if (transform)
			{
				renderTransform = transform->GetRenderTransform();
			}

			SetConstantBuffer(m_device, m_vsConstantBuffers, 2, &renderTransform, sizeof(RenderTransform));
			ApplyConstantBuffers<ID3D11VertexShader>();

			std::shared_ptr<StaticMeshComponent> mesh = wireframeMesh->GetMesh();
			Direct3dVertexBufferf vertexBuffer = mesh->GetVertexBuffer();

			BindVertexBuffer(vertexBuffer);

			for (std::shared_ptr<VertexGroup> group : mesh->GetVertexGroups())
			{
				if (group->PixelShader)
					m_device->UseShader(group->PixelShader->GetShaderPtr().get(), nullptr, 0);
				else
					m_device->UseShader(m_shaderProgram->PixelShader->GetShaderPtr().get(), nullptr, 0);

				SetConstantBuffer(m_device, m_psConstantBuffers, 0, &group->PSData, sizeof(PixelShaderData));

				ApplyConstantBuffers<ID3D11PixelShader>();
				m_device->DrawIndexed(group->IndexCount, group->StartIndexLocation, 0);
			}
		}

		void WireframeRendererSystem::BindVertexBuffer(const dsr::directX::Direct3dVertexBufferf& vertexBuffer)
		{
			using namespace dsr::directX;

			Direct3dBuffer vb = vertexBuffer.GetVertexBuffer();
			Direct3dShaderInputLayout layout = vertexBuffer.GetLayout();
			uint32_t vertexStride = layout.GetTotalStride();
			uint32_t offset = 0;

			std::shared_ptr<ID3D11Buffer> vertexBufferPtr = vb.GetBufferPtr();
			ID3D11Buffer* vertexShaderRawPtr = vertexBufferPtr.get();

			m_device->SetVertexBuffers(0, 1, &vertexShaderRawPtr, &vertexStride, &offset);

			std::shared_ptr<ID3D11Buffer> indexBufferPtr = vertexBuffer.GetIndexBuffer().GetBufferPtr();
			m_device->SetIndexBuffer(indexBufferPtr.get());
		}
	}
}
