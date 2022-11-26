#pragma once
#include "Line.hpp"

#include <list>
#include <stack>
#include <string_view>

namespace mlc
{
	// Used for critical errors that determine the type of line
	struct CriticalError { explicit CriticalError() = default; };

	class Error
	{
	public:

		Error(const mlc::Line& line, const std::wstring_view message) noexcept;
		Error(const mlc::Line& line) noexcept;
		Error(const CriticalError& critical) noexcept;
		Error() noexcept;

		// Print error
		friend std::wostream& operator<<(std::wostream& os, const Error& err) noexcept;

		// If this an error empty
		operator bool() const noexcept;
		bool empty() const noexcept;

		const std::wstring& str() const noexcept;
		// Make an error empty
		void clear() noexcept;

		// If this error is critical
		bool critical() const noexcept;

	private:
		std::wstring m_str;
		bool m_critical;
	};
	std::wostream& operator<<(std::wostream& os, const Error& err) noexcept;

	class ErrorTrace
	{
	public:

		using container_type = std::list<mlc::Error>;
		using size_type = typename container_type::size_type;

		ErrorTrace() noexcept;
		ErrorTrace(std::initializer_list<mlc::Error> list) noexcept;

		// Add error to trace && if the error is empty do nothing
		const mlc::Error& push(const mlc::Error& error) noexcept;
		// Combine the two trace
		const mlc::ErrorTrace& push(const mlc::ErrorTrace& trace) noexcept;

		// Remove and return last error
		mlc::Error pop() noexcept;

		size_type size() const noexcept;

		// Check if trace have not empty errors
		bool empty() const noexcept;
		operator bool() const noexcept;

		bool critical() const noexcept;
		
		// Get raw container
		container_type& container() noexcept;
		const container_type& container() const noexcept;

		// Clear trace
		void clear() noexcept;

		// Print errors
		friend std::wostream& operator<<(std::wostream& os, const ErrorTrace& errtrace) noexcept;

	private:
		container_type m_trace;
	};
	std::wostream& operator<<(std::wostream& os, const ErrorTrace& errtrace) noexcept;
}

