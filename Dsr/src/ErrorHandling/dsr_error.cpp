
#include <dsrpch.h>
#include "dsr_error.h"

namespace dsr
{
	dsr_error dsr_error::Attach(const std::string& summary, const dsr_error& previous)
	{
		return dsr_error(summary + previous.what(), previous.GetResult());
	}
}
