#pragma once

namespace dsr
{
	class createdirecd3ddevice_error : public std::exception
	{
	public:
		HRESULT GetResult() const { return m_result; }

		createdirecd3ddevice_error(
			const std::string& message,
			const HRESULT& result)
			: exception(message.c_str()), m_result(result)
		{
		}
	private:
		HRESULT m_result;
	};
}
