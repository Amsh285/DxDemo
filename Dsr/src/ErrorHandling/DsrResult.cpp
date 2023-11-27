#include "dsrpch.h"
#include "DsrResult.h"

namespace dsr
{
	DsrResult DsrResult::Success(const std::string& message)
	{
		return DsrResult(message, RESULT_SUCCESS);
	}

	DsrResult::DsrResult(const std::string& message, const int32_t& statusCode)
		: m_message(message), m_statusCode(statusCode)
	{
	}
}
