#pragma once

#include "EngineSubSystems/EntityComponentSystem/Entity.h"

namespace dsr
{
	namespace scenesystem
	{
		class Scene
		{
		public:
			std::wstring GetName() const { return m_name; }
			void SetName(const std::wstring& name) { m_name = name; }


		private:
			std::wstring m_name;
		};
	}
}
