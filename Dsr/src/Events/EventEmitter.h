#pragma once

#include "EventRegister.h"

namespace dsr
{
	namespace events
	{
		template<class TObject, class TEvent>
		class EventEmitter : public EventRegister<TObject, TEvent>
		{
		public:
			void operator()(TEvent e) const
			{
				for (const std::pair<std::shared_ptr<TObject>, EventRegister<TObject, TEvent>::EventDelegate>& callback : this->m_callbacks)
					(callback.first.get()->*(callback.second))(e);
			}
		};
	}
}
