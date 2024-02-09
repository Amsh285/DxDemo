#include "dsrpch.h"
#include "LineListRendererSystem.h"



namespace dsr
{
	namespace ecs
	{
		std::vector<std::type_index> LineListRendererSystem::GetRequiredComponents() const
		{
			return { std::type_index(typeid(LineListComponent)) };
		}

		LineListRendererSystem::LineListRendererSystem(const std::shared_ptr<directX::Direct3dDevice>& device)
			: RendererSystem(std::type_index(typeid(LineListRendererSystem))),
			m_device(device)
		{
			OnStart = std::bind(&LineListRendererSystem::Startup, this, std::placeholders::_1);
			OnUpdate = std::bind(&LineListRendererSystem::Update, this, std::placeholders::_1);
			OnPrepareRendererUpdate = std::bind(&LineListRendererSystem::PrepareRendererUpdate, this, std::placeholders::_1);
		}

		void LineListRendererSystem::Startup(const EngineStartupContext& context)
		{
			std::vector<Entity> matches = context.FindEntitiesByTag("LineListShaderProgram");

			if (matches.empty())
			{
				//Todo: warn here when we have a logging framework.
				return;
			}
				
			std::shared_ptr<ShaderProgramComponent> program = context.GetComponentFrom<ShaderProgramComponent>(matches[0]);

			if (!program)
			{
				//Todo: same
				return;
			}

			m_shaderProgram = program->GetShaderProgram();
		}

		void LineListRendererSystem::PrepareRendererUpdate(const EnginePrepareRendererContext& context)
		{
			m_device->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

			std::vector<Entity> cameras = context.FindEntitiesByTag("Camera");

			DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();

			if (!cameras.empty())
			{
				std::shared_ptr<ViewProjectionComponent> viewProjection = context.GetComponentFrom<ViewProjectionComponent>(cameras[0]);
				projectionMatrix = viewProjection->GetProjectionMatrix();
				viewMatrix = viewProjection->GetViewMatrix();
			}
			else
			{
				//Todo: log warn...
			}

			SetConstantBuffer(m_device, m_vsConstantBuffers, 0, projectionMatrix);
			SetConstantBuffer(m_device, m_vsConstantBuffers, 1, viewMatrix);

			if (m_shaderProgram)
			{
				m_device->SetInputLayout(m_shaderProgram->VertexShaderInputLayout.get());
				m_device->UseShader(m_shaderProgram->VertexShader->GetShaderPtr().get(), nullptr, 0);
				m_device->UseShader(m_shaderProgram->PixelShader->GetShaderPtr().get(), nullptr, 0);
			}
		}

		void LineListRendererSystem::Update(const EngineContext& context)
		{
			using namespace dsr::directX;

			std::shared_ptr<TransformComponent> transform = context.GetComponent<TransformComponent>();
			std::shared_ptr<LineListComponent> lines = context.GetComponent<LineListComponent>();
			
			RenderTransform renderTransform;

			if (transform)
			{
				renderTransform = transform->GetRenderTransform();
			}
				
			SetConstantBuffer(m_device, m_vsConstantBuffers, 2, &renderTransform, sizeof(RenderTransform));
			ApplyConstantBuffers<ID3D11VertexShader>();


			Direct3dBuffer vertexBuffer = lines->GetVertexBuffer();
			Direct3dShaderInputLayout layout = lines->GetVertexShaderInputLayout();
			uint32_t vertexStride = layout.GetTotalStride();
			uint32_t offset = 0;

			std::shared_ptr<ID3D11Buffer> vertexBufferPtr = vertexBuffer.GetBufferPtr();
			ID3D11Buffer* vertexShaderRawPtr = vertexBufferPtr.get();

			m_device->SetVertexBuffers(0, 1, &vertexShaderRawPtr, &vertexStride, &offset);
			m_device->Draw(lines->GetVertexCount(), lines->GetStartVertexLocation());
		}
	}
}
