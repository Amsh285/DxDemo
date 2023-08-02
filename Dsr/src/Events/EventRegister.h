#pragma once

namespace dsr
{
	namespace events
	{
		template<class TObject, class TEvent>
		class EventRegister
		{
		public:
			using EventDelegate = void(TObject::*)(TEvent);

			EventRegister(){}
			EventRegister(const EventRegister<TObject, TEvent>& other) = delete;
			EventRegister<TObject, TEvent>& operator=(const EventRegister<TObject, TEvent>& other) = delete;
			virtual ~EventRegister(){}

			template<class TSubType>
			void Hook(const std::shared_ptr<TObject>& instance, void((TSubType::* func)(TEvent)))
			{
				assert(instance);
				assert(func);

				auto del = static_cast<EventDelegate>(func);
				m_callbacks.push_back(std::make_pair(instance, del));
			}

			void Hook(const std::shared_ptr<TObject>& instance, EventDelegate func)
			{
				assert(instance);
				assert(func);

				m_callbacks.push_back(std::make_pair(instance, func));
			}

			template<class TSubType>
			void UnHook(const std::shared_ptr<TObject>& instance, void((TSubType::* func)(TEvent)))
			{
				assert(instance);
				assert(func);

				auto del = static_cast<EventDelegate>(func);
				UnHook(instance, del);
			}

			void UnHook(const std::shared_ptr<TObject>& instance, EventDelegate func)
			{
				assert(instance);
				assert(func);

				auto pred = [&](const std::pair<std::shared_ptr<TObject>, EventDelegate>& p) {
					return p.first == instance && p.second == func;
				};

				auto it = std::find_if(m_callbacks.begin(), m_callbacks.end(), pred);

				if (it != m_callbacks.end())
					m_callbacks.erase(it);
			}

			void UnHookAll(const std::shared_ptr<TObject>& instance)
			{
				assert(instance);

				auto pred = [&](const std::pair<std::shared_ptr<TObject>, EventDelegate>& p) {
					return p.first == instance;
				};

				m_callbacks.erase(
					std::remove_if(
						m_callbacks.begin(),
						m_callbacks.end(),
						pred),
					m_callbacks.end()
				);
			}

		protected:
			std::vector<std::pair<std::shared_ptr<TObject>, EventDelegate>> m_callbacks;
		};
	}
}
