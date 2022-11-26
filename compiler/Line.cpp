#include "Line.hpp"

mlc::Line::operator value_type& () & noexcept {
	return m_data;
}

mlc::Line::operator const value_type& () const& noexcept {
	return m_data;
}

mlc::Line::Line() noexcept
	: m_data(), m_counter(0)
{}

mlc::Line::Line(const std::wstring_view data, line_counter_type counter) noexcept
	: m_data(data), m_counter(counter)
{}

mlc::Line::Line(const std::wstring_view data, const Line& other) noexcept
	: m_data(data), m_counter(other.m_counter)
{}

mlc::Line::value_type& mlc::Line::get() noexcept {
	return m_data;
}

const mlc::Line::value_type& mlc::Line::get() const noexcept {
	return m_data;
}

mlc::Line::line_counter_type mlc::Line::line() const noexcept {
	return m_counter;
}

mlc::Line& mlc::Line::operator++() noexcept {
	++m_counter;
	return *this;
}

//mlc::Line mlc::Line::operator++(int) noexcept {
//	auto tmp(*this);
//	++m_counter;
//	return tmp;
//}

mlc::Line& mlc::Line::operator--() noexcept {
	--m_counter;
	return *this;
}

//mlc::Line mlc::Line::operator-(int) noexcept {
//	auto tmp(*this);
//	--m_counter;
//	return tmp;
//}
