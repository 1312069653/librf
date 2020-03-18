#pragma once

RESUMEF_NS
{
	enum struct error_code
	{
		none,
		not_ready,			// get_value called when value not available
		already_acquired,	// attempt to get another future
		unlock_more,		// unlock ��������lock����
		read_before_write,	// 0������channel���ȶ���д
		timer_canceled,		// ��ʱ��������ȡ��
		not_await_lock,		// û����Э����ʹ��co_await�ȴ�lock���

		max__
	};

	const char* get_error_string(error_code fe, const char* classname);

	struct future_exception : std::logic_error
	{
		error_code _error;
		future_exception(error_code fe)
			: logic_error(get_error_string(fe, "future_exception"))
			, _error(fe)
		{
		}
	};

	struct lock_exception : std::logic_error
	{
		error_code _error;
		lock_exception(error_code fe)
			: logic_error(get_error_string(fe, "lock_exception"))
			, _error(fe)
		{
		}
	};

	struct channel_exception : std::logic_error
	{
		error_code _error;
		channel_exception(error_code fe)
			: logic_error(get_error_string(fe, "channel_exception"))
			, _error(fe)
		{
		}
	};

	struct timer_canceled_exception : public std::logic_error
	{
		error_code _error;
		timer_canceled_exception(error_code fe)
			: logic_error(get_error_string(fe, "timer canceled"))
			, _error(fe)
		{
		}
	};
}