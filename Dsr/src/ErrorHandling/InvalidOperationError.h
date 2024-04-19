#pragma once

namespace dsr
{
	class InvalidOperationError : public std::exception
	{
	public:
		InvalidOperationError(const std::string& message);
	private:
	};
}
