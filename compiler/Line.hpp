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

		line_counter_type line() const noexcept;

		Line& operator++() noexcept;
		Line operator++(int) noexcept;
		Line& operator--() noexcept;
		Line operator-(int) noexcept;

	private:
		value_type m_data;
		line_counter_type m_counter;
	};

}
