#include "dsrpch.h"
#include "InvalidOperationError.h"

namespace dsr
{
	InvalidOperationError::InvalidOperationError(const std::string& message)
		: exception(message.c_str())
	{
	}
}
