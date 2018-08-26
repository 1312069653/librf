//Э�̵Ķ�ʱ��
//�����ʱ����ȡ���ˣ������ timer_canceled_exception �쳣
//��ʹ��co_await������sleep_for/sleep_until���Ǵ�����÷��������ܴﵽ�ȴ���Ŀ�ġ��������������һ����Ч�Ķ�ʱ��������ޱ�Ҫ���ڴ�ʹ��
//
#pragma once

namespace resumef
{
	struct scheduler;

	RF_API future_vt sleep_until_(const std::chrono::system_clock::time_point& tp_, scheduler & scheduler_);

	inline future_vt sleep_for_(const std::chrono::system_clock::duration& dt_, scheduler & scheduler_)
	{
		return std::move(sleep_until_(std::chrono::system_clock::now() + dt_, scheduler_));
	}

	template<class _Rep, class _Period>
	future_vt sleep_for(const std::chrono::duration<_Rep, _Period>& dt_, scheduler & scheduler_)
	{
		return std::move(sleep_for_(std::chrono::duration_cast<std::chrono::system_clock::duration>(dt_), scheduler_));
	}
	template<class _Rep, class _Period>
	future_vt sleep_for(const std::chrono::duration<_Rep, _Period>& dt_)
	{
		return std::move(sleep_for_(std::chrono::duration_cast<std::chrono::system_clock::duration>(dt_), *this_scheduler()));
	}

	template<class _Clock, class _Duration = typename _Clock::duration>
	future_vt sleep_until(const std::chrono::time_point<_Clock, _Duration>& tp_, scheduler & scheduler_)
	{
		return std::move(sleep_until_(std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp_), scheduler_));
	}
	template<class _Clock, class _Duration>
	future_vt sleep_until(const std::chrono::time_point<_Clock, _Duration>& tp_)
	{
		return std::move(sleep_until_(std::chrono::time_point_cast<std::chrono::system_clock::duration>(tp_), *this_scheduler()));
	}

	template <class Rep, class Period>
	auto operator co_await(std::chrono::duration<Rep, Period> dt_)
	{
		return sleep_for(dt_);
	}

}