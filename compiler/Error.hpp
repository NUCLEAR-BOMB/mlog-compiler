#pragma once
#include "Line.hpp"

#include <list>
#include <stack>
#include <string_view>

namespace mlc
{
	struct CriticalError { explicit CriticalError() = default; };

	class Error
	{
	public:

		Error(const mlc::Line& line, const std::string_view message) noexcept;
		Error(const mlc::Line& line) noexcept;
		Error(const CriticalError& critical) noexcept;
		Error() noexcept;

		friend std::ostream& operator<<(std::ostream& os, const Error& err) noexcept;

		operator bool() const noexcept;
		bool empty() const noexcept;

		const std::string& str() const noexcept;
		void clear() noexcept;

		bool critical() const noexcept;

	private:
		std::string m_str;
		bool m_critical;
	};
	std::ostream& operator<<(std::ostream& os, const Error& err) noexcept;

	class ErrorTrace
	{
	public:

		using container_type = std::list<mlc::Error>;
		using size_type = typename container_type::size_type;

		ErrorTrace() noexcept;
		ErrorTrace(std::initializer_list<mlc::Error> list) noexcept;

		const mlc::Error& push(const mlc::Error& error) noexcept;
		const mlc::ErrorTrace& push(const mlc::ErrorTrace& trace) noexcept;

		mlc::Error pop() noexcept;

		size_type size() const noexcept;

		bool empty() const noexcept;
		operator bool() const noexcept;

		container_type& container() noexcept;
		const container_type& container() const noexcept;

		void clear() noexcept;

		friend std::ostream& operator<<(std::ostream& os, const ErrorTrace& errtrace) noexcept;

	private:
		container_type m_trace;
	};
	std::ostream& operator<<(std::ostream& os, const ErrorTrace& errtrace) noexcept;
}

