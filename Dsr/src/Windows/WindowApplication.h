#pragma once

namespace dsr
{
	namespace windows
	{
		class WindowApplication
		{
		public:
			static std::shared_ptr<WindowApplication> Get();

			void PeekMessages();
			void ShutDown();
		private:
			WindowApplication()
			{
			}

			static std::shared_ptr<WindowApplication> m_instance;
			static std::mutex m_appMutex;
		};
	}
}
