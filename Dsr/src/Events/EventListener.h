#pragma once

namespace dsr
{
	namespace events
	{
		//Marker-class for types subscribing to EventEmitters
		class EventListener
		{
		public:
			virtual ~EventListener() = default;
		};
	}
}
