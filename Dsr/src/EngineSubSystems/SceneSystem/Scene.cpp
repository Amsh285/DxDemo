#include "dsrpch.h"
#include "Scene.h"

namespace dsr
{
	namespace scenesystem
	{
		Scene::Scene()
		{
			static std::atomic<uint32_t> uid = 1;

			m_id = uid.fetch_add(1);
		}

		Scene::Scene(const std::string& name)
			: Scene()
		{
			m_name = name;
		}
	}
}
