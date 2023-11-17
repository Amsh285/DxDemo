#pragma once

namespace dsr
{
#define RESULT_SUCCESS 0

	class DsrResult
	{
	public:
		std::string GetResultMessage() const { return m_message; }
		int32_t GetResultStatusCode() const { return m_statusCode; }

		static DsrResult Success(const std::string& message);

		DsrResult(const std::string& message, const int32_t& statusCode);
	private:
		std::string m_message;
		int32_t m_statusCode;
	};
}
