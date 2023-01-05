#pragma once

#include "TypeAliases.hpp"

#include <cstdint>

namespace mlc
{

class Line
{
public:

	using line_number_type = std::uint_fast32_t;

	explicit Line(line_number_type linenum, mlc::string_t str) noexcept;
	explicit Line(mlc::string_t str) noexcept;

	Line() noexcept;

	mlc::string_ref_t str() noexcept;

	line_number_type line_number() const noexcept;
	line_number_type& line_number() noexcept;

	mlc::string_const_ref_t str() const noexcept;

	bool is_empty() const noexcept;

	void uncomment(mlc::char_t_ sym) noexcept;

	Line pop_subline(mlc::char_t_ delim) noexcept;

private:
	line_number_type m_linenum;
	mlc::string_t m_str;
};

}
