#pragma once

namespace dsr
{
	class dsr_error : public std::exception
	{
	public:
		static dsr_error Attach(const std::string& summary, const dsr_error& previous);

		HRESULT GetResult() const { return m_result; }

		dsr_error(
			const std::string& message,
			const HRESULT& result)
			: exception(message.c_str()), m_result(result)
		{
		}
	private:
		HRESULT m_result;
	};
}
