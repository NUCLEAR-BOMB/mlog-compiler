#pragma once

#include "Line.hpp"
#include <utility>

mlc::Line::Line(line_number_type linenum, mlc::string_t str) noexcept
	: m_linenum(linenum), m_str(std::move(str))
{}

mlc::Line::Line(mlc::string_t str) noexcept
	: m_linenum(UINT_FAST32_MAX), m_str(std::move(str))
{}

mlc::Line::Line() noexcept
	: m_linenum(UINT_FAST32_MAX), m_str("")
{
}

mlc::string_ref_t mlc::Line::str() noexcept {
	return m_str;
}

mlc::Line::line_number_type mlc::Line::line_number() const noexcept {
	return m_linenum;
}

mlc::Line::line_number_type& mlc::Line::line_number() noexcept {
	return m_linenum;
}

mlc::string_const_ref_t mlc::Line::str() const noexcept {
	return m_str;
}

bool mlc::Line::is_empty() const noexcept {
	return (m_str.find_first_not_of("\t\n ") == mlc::string_t::npos);
}

void mlc::Line::uncomment(mlc::char_t_ sym) noexcept
{
	const auto comment_begin = m_str.find_first_of(sym);

	m_str.erase(comment_begin, mlc::string_t::npos);
}

mlc::Line mlc::Line::pop_subline(mlc::char_t_ delim) noexcept
{
	const auto subline_end = m_str.find_first_of(delim);

	mlc::string_t subline_ = m_str.substr(0, subline_end);

	m_str.erase(0, subline_end + 1);

	return mlc::Line(m_linenum, subline_);
}
