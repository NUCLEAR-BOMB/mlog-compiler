#pragma once

#include <string>

namespace mlc
{
	class Line
	{
	public:

		using value_type = std::string;
		using line_counter_type = std::size_t;

		operator value_type&() & noexcept;
		operator const value_type&() const& noexcept;
		//operator value_type() && noexcept;

		// Get string from line
		value_type& get() noexcept;
		const value_type& get() const noexcept;

		// Get line number
		line_counter_type line() const noexcept;

		// Increase the line number by one
		Line& operator++() noexcept;
		//Line operator++(int) noexcept;

		// Reduce the line number by one
		Line& operator--() noexcept;
		//Line operator-(int) noexcept;

	private:
		value_type m_data;
		line_counter_type m_counter = 0;
	};

}
