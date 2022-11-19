#pragma once
#include "Line.hpp"

#include <list>
#include <stack>
#include <string_view>

namespace mlc
{
	class Error
	{
	public:

		Error(const mlc::Line& line, const std::string_view message) noexcept;
		Error(const mlc::Line& line) noexcept;

		friend std::ostream& operator<<(std::ostream os, const Error& err) noexcept;

		operator bool() const noexcept;

		const std::string& str() const noexcept;

	private:
		std::string m_str;
	};

	class ErrorTrace
	{
	public:

		using container_type = std::stack<mlc::Error, std::list<mlc::Error>>;
		using size_type = typename container_type::size_type;

		ErrorTrace(std::initializer_list<mlc::Error> list) noexcept;

		void push(const mlc::Error& error) noexcept;
		mlc::Error pop() noexcept;

		size_type size() const noexcept;
		bool empty() const noexcept;

	private:
		container_type m_trace;
	};
}

