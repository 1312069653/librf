#pragma once

RESUMEF_NS
{
	namespace traits
	{
		//is_coroutine_handle<T>
		//is_coroutine_handle_v<T>
		//�ж��ǲ���coroutine_handle<>����
		//
		//is_valid_await_suspend_return_v<T>
		//�ж��ǲ���awaitor��await_suspend()��������Ч����ֵ
		//
		//is_awaitor<T>
		//is_awaitor_v<T>
		//�ж��ǲ���һ��awaitor�淶��
		//һ��awaitor���Ա�co_await������Ҫ������coroutine��awaitor�����������ӿڹ淶
		//
		//is_future<T>
		//is_future_v<T>
		//�ж��ǲ���һ��librf��future�淶��
		//future����Ҫ����һ��awaitor�⣬��Ҫ������value_type/state_type/promise_type�������ͣ�
		//���Ҿ߱�counted_ptr<state_type>���͵�_state������
		//
		//is_promise<T>
		//is_promise_v<T>
		//�ж��ǲ���һ��librf��promise_t��
		//
		//is_generator<T>
		//is_generator_v<T>
		//�ж��ǲ���һ��librf��generator_t��
		//
		//has_state<T>
		//has_state_v<T>
		//�ж��Ƿ����_state�ĳ�Ա����
		//
		//get_awaitor<T>(T&&t)
		//ͨ��T����䱻co_await���awaitor
		//
		//awaitor_traits<T>
		//���һ��awaitor��������
		//	type:awaitor������
		//	value_type:awaitor::await_resume()�ķ���ֵ����
		//
		//is_callable<T>
		//is_callable_v<T>
		//�ж��ǲ���һ���ɱ����õ����ͣ��纯�����º�����lambda��
		//
		//is_scheduler_task<T>
		//is_scheduler_task_v<T>
		//�ж��ǲ��ǿ��Ա����������ȵ����񡣵�����֧��future��callable

		template<class _Ty>
		struct is_coroutine_handle : std::false_type {};
		template<class _PromiseT>
		struct is_coroutine_handle<coroutine_handle<_PromiseT>> : std::true_type {};
		template<class _Ty>
		constexpr bool is_coroutine_handle_v = is_coroutine_handle<remove_cvref_t<_Ty>>::value;

		template<class _Ty>
		constexpr bool is_valid_await_suspend_return_v = std::is_void_v<_Ty> || std::is_same_v<_Ty, bool> || is_coroutine_handle_v<_Ty>;

		template<class _Ty, class = std::void_t<>>
		struct is_awaitor : std::false_type {};
		template<class _Ty>
		struct is_awaitor
			<_Ty,
				std::void_t<
					decltype(std::declval<_Ty>().await_ready())
					, decltype(std::declval<_Ty>().await_suspend(std::declval<std::experimental::coroutine_handle<promise_t<>>>()))
					, decltype(std::declval<_Ty>().await_resume())
				>
			>
			: std::bool_constant<
				std::is_constructible_v<bool, decltype(std::declval<_Ty>().await_ready())>
				&& is_valid_await_suspend_return_v<
					decltype(std::declval<_Ty>().await_suspend(std::declval<std::experimental::coroutine_handle<promise_t<>>>()))
				>
			>
		{};
		template<class _Ty>
		struct is_awaitor<_Ty&> : is_awaitor<_Ty> {};
		template<class _Ty>
		struct is_awaitor<_Ty&&> : is_awaitor<_Ty> {};

		template<class _Ty>
		constexpr bool is_awaitor_v = is_awaitor<remove_cvref_t<_Ty>>::value;

		template<class _Ty, class = std::void_t<>>
		struct is_future : std::false_type {};
		template<class _Ty>
		struct is_future<_Ty,
				std::void_t<
					decltype(std::declval<_Ty>()._state),
					typename _Ty::value_type,
					typename _Ty::state_type,
					typename _Ty::promise_type
				>
			> : std::true_type {};
		template<class _Ty>
		constexpr bool is_future_v = is_future<remove_cvref_t<_Ty>>::value;

		template<class _Ty>
		struct is_promise : std::false_type {};
		template<class _Ty>
		struct is_promise<promise_t<_Ty>> : std::true_type {};
		template<class _Ty>
		constexpr bool is_promise_v = is_promise<remove_cvref_t<_Ty>>::value;

		template<class _Ty>
		struct is_generator : std::false_type {};
		template <class _Ty, class _Alloc>
		struct is_generator<generator_t<_Ty, _Alloc>> : std::true_type {};
		template<class _Ty>
		constexpr bool is_generator_v = is_generator<remove_cvref_t<_Ty>>::value;

		template<class _Ty, class = std::void_t<>>
		struct is_state_pointer : std::false_type {};
		template<class _Ty>
		struct is_state_pointer<_Ty, std::void_t<std::enable_if_t<std::is_convertible_v<_Ty, state_base_t*>>>> : std::true_type {};
		template<class _Ty>
		struct is_state_pointer<counted_ptr<_Ty>> : is_state_pointer<_Ty> {};
		template<class _Ty>
		constexpr bool is_state_pointer_v = is_state_pointer<remove_cvref_t<_Ty>>::value;


		template<class _Ty, class = std::void_t<>>
		struct has_state : std::false_type {};
		template<class _Ty>
		struct has_state<_Ty, std::void_t<decltype(std::declval<_Ty>()._state)>> : std::true_type {};
		template<class _Ty>
		constexpr bool has_state_v = has_state<remove_cvref_t<_Ty>>::value;


		//copy from cppcoro
		namespace detail
		{
			template<class T>
			auto get_awaitor_impl(T&& value, int) noexcept(noexcept(static_cast<T&&>(value).operator co_await()))
				-> decltype(static_cast<T&&>(value).operator co_await())
			{
				return static_cast<T&&>(value).operator co_await();
			}
			template<class T>
			auto get_awaitor_impl(T&& value, long) noexcept(noexcept(operator co_await(static_cast<T&&>(value))))
				-> decltype(operator co_await(static_cast<T&&>(value)))
			{
				return operator co_await(static_cast<T&&>(value));
			}
			template<class T, std::enable_if_t<is_awaitor_v<T&&>, int> = 0>
			T&& get_awaitor_impl(T&& value, std::any) noexcept
			{
				return static_cast<T&&>(value);
			}
		}

		template<class T>
		auto get_awaitor(T&& value) noexcept(noexcept(detail::get_awaitor_impl(static_cast<T&&>(value), 123)))
			-> decltype(detail::get_awaitor_impl(static_cast<T&&>(value), 123))
		{
			return detail::get_awaitor_impl(static_cast<T&&>(value), 123);
		}

		template<class _Ty, class = std::void_t<>>
		struct awaitor_traits{};

		template<class _Ty>
		struct awaitor_traits<_Ty, 
			std::void_t<decltype(get_awaitor(std::declval<_Ty>()))>
		>
		{
			using type = decltype(get_awaitor(std::declval<_Ty>()));
			using value_type = decltype(std::declval<type>().await_resume());
		};

		template<typename _Ty, class = std::void_t<>>
		struct is_callable : std::false_type{};
		template<typename _Ty>
		struct is_callable<_Ty, std::void_t<decltype(std::declval<_Ty>()())>> : std::true_type {};
		template<typename _Ty>
		constexpr bool is_callable_v = is_callable<_Ty>::value;

		template<class _Ty, class = std::void_t<>>
		struct is_iterator : std::false_type {};
		template<class _Ty>
		struct is_iterator
			<_Ty,
				std::void_t<
					decltype(++std::declval<_Ty>())
					, decltype(std::declval<_Ty>() != std::declval<_Ty>())
					, decltype(*std::declval<_Ty>())
				>
			>
			: std::true_type{};
		template<class _Ty>
		constexpr bool is_iterator_v = is_iterator<remove_cvref_t<_Ty>>::value;

		template<class _Ty, class = std::void_t<>>
		struct is_container : std::false_type {};
		template<class _Ty>
		struct is_container
			<_Ty,
				std::void_t<
					decltype(std::begin(std::declval<_Ty>()))
					, decltype(std::end(std::declval<_Ty>()))
				>
			>
			: is_iterator<decltype(std::begin(std::declval<_Ty>()))> {};
		template<class _Ty>
		constexpr bool is_container_v = is_container<remove_cvref_t<_Ty>>::value;
	}
}
