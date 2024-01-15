#pragma once

namespace dsr
{
	namespace events
	{
		//Marker-class for Events
		class IEvent
		{
		public:
			virtual ~IEvent() = default;
		};
	}
}
