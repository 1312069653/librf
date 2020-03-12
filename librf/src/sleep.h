//Э�̵Ķ�ʱ��
//�����ʱ����ȡ���ˣ������ timer_canceled_exception �쳣
//��ʹ��co_await������sleep_for/sleep_until���Ǵ�����÷��������ܴﵽ�ȴ���Ŀ�ġ��������������һ����Ч�Ķ�ʱ��������ޱ�Ҫ���ڴ�ʹ��
//
#pragma once

RESUMEF_NS
{
	struct scheduler_t;

	future_t<> sleep_until_(const std::chrono::system_clock::time_point& tp_, scheduler_t& scheduler_);

	inline future_t<> sleep_for_(const std::chrono::system_clock::duration& dt_, scheduler_t& scheduler_)
	{
		return sleep_until_(std::chrono::system_clock::now() + dt_, scheduler_);
	}

	template<class _Rep, class _Period>
	inline future_t<> sleep_for(const std::chrono::duration<_Rep, _Period>& dt_, scheduler_t& scheduler_)
	{
		return sleep_for_(std::chrono::duration_cast<std::chrono::system_clock::duration>(dt_), scheduler_);
	}

	template<class _Clock, class _Duration = typename _Clock::duration>
	inline future_t<> sleep_until(const std::chrono::time_point<_Clock, _Duration>& tp_, scheduler_t& scheduler_)
	{
		return sleep_until_(std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp_), scheduler_);
	}

	template<class _Rep, class _Period>
	inline future_t<> sleep_for(const std::chrono::duration<_Rep, _Period>& dt_)
	{
		co_await sleep_for_(std::chrono::duration_cast<std::chrono::system_clock::duration>(dt_), *current_scheduler());
	}
	template<class _Clock, class _Duration>
	inline future_t<> sleep_until(const std::chrono::time_point<_Clock, _Duration>& tp_)
	{
		co_await sleep_until_(std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp_), *current_scheduler());
	}

	template <class Rep, class Period>
	inline future_t<> operator co_await(std::chrono::duration<Rep, Period> dt_)
	{
		co_await sleep_for(dt_, *current_scheduler());
	}

}