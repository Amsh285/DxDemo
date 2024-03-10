#pragma once

#include "EngineSubSystems/EntityComponentSystem/Component.h"
#include "EngineSubSystems/EntityComponentSystem/Components/StaticMeshComponent.h"

namespace dsr
{
	namespace ecs
	{
		class WireframeMeshComponent final : public Component
		{
		public:
			std::shared_ptr<StaticMeshComponent> GetMesh() const { return m_mesh; }
			void SetMesh(const std::shared_ptr<StaticMeshComponent>& mesh) { m_mesh = mesh; }
		private:
			std::shared_ptr<StaticMeshComponent> m_mesh;
		};
	}
}
