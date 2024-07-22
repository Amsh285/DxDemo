#pragma once

namespace dsr
{
	template<class T>
	class SyncHandle
	{
	public:
		uint32_t GetSyncAccessId() const { return m_syncAccessId; }

		const T& GetData() const { return m_data; }
		void SetData(const T& data) { m_data = data; }

		const std::chrono::time_point<std::chrono::steady_clock>& GetTimestamp() const { return m_timestamp; }
		void SetTimestamp(const std::chrono::time_point<std::chrono::steady_clock>& timestamp) { m_timestamp = timestamp; }

		SyncHandle()
			: m_syncAccessId(std::numeric_limits<uint32_t>::max()), m_data(T()), m_timestamp(std::chrono::steady_clock::now())
		{
		}

		SyncHandle(const uint32_t syncAccessId, const T& data, const std::chrono::time_point<std::chrono::steady_clock>& timestamp)
			: m_syncAccessId(syncAccessId), m_data(std::move(data)), m_timestamp(timestamp)
		{
		}
	private:
		uint32_t m_syncAccessId;
		T m_data;
		std::chrono::time_point<std::chrono::steady_clock> m_timestamp;
	};

	template<class T>
	class SyncAccess
	{
	public:
		uint32_t GetId() const { return m_id; }

		SyncAccess()
			: m_timestamp(std::chrono::steady_clock::now()), m_data(T())
		{
			static std::atomic<uint32_t> idCounter = 0;

			m_id = idCounter.fetch_add(1);
		}

		SyncHandle<T> CreateHandle() const
		{
			std::shared_lock<std::shared_mutex> lock(m_mutex);
			return SyncHandle<T>(m_id, m_data, m_timestamp);
		}

		void SetData(const T& data)
		{
			std::unique_lock<std::shared_mutex> lock(m_mutex);
			m_data = data;
			m_timestamp = std::chrono::steady_clock::now();
		}

		void Update(SyncHandle<T>& handle) const
		{
			assert(handle.GetSyncAccessId() == m_id);

			std::shared_lock<std::shared_mutex> lock(m_mutex);

			if (handle.GetTimestamp() == m_timestamp)
				return;

			handle.SetData(m_data);
			handle.SetTimestamp(m_timestamp);
		}
	private:
		mutable std::shared_mutex m_mutex;

		uint32_t m_id;
		std::chrono::time_point<std::chrono::steady_clock> m_timestamp;
		T m_data;
	};
}
