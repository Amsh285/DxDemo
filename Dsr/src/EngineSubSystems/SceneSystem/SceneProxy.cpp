#include "dsrpch.h"
#include "SceneProxy.h"

namespace dsr
{
	namespace scenesystem
	{
		SceneProxy::SceneProxy(const std::shared_ptr<Scene>& scene, const std::shared_ptr<dsr::ecs::EcsManager>& ecsManager)
			: m_scene(scene), m_ecsManager(ecsManager)
		{
		}
	}
}
