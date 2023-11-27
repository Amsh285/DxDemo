#pragma once

namespace dsr
{
	class NotFoundError : public std::exception
	{
	public:
		NotFoundError(const std::string& message)
			: exception(message.c_str())
		{
		}
	private:
	};
}
