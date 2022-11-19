#include "Error.hpp"

#include <sstream>

mlc::Error::Error(const mlc::Line& line, const std::string_view message) noexcept {
	std::stringstream ss;
	ss << "Line " << line.line() << " [Error] | " << line.get() << " / " << message;
	m_str = ss.str();
}

mlc::Error::Error(const mlc::Line& line) noexcept
{
	std::stringstream ss;
	ss << "Line " << line.line() << " [Error] | " << line.get();
	m_str = ss.str();
}

mlc::Error::operator bool() const noexcept {
	return m_str.empty();
}

const std::string& mlc::Error::str() const noexcept {
	return m_str;
}

std::ostream& mlc::operator<<(std::ostream os, const Error& err) noexcept {
	os << err.m_str;
	return os;
}

mlc::ErrorTrace::ErrorTrace(std::initializer_list<mlc::Error> list) noexcept
	: m_trace(list)
{}

void mlc::ErrorTrace::push(const mlc::Error& error) noexcept {
	m_trace.push(error);
}

mlc::Error mlc::ErrorTrace::pop() noexcept {
	mlc::Error tmp = m_trace.top();
	m_trace.pop();
	return tmp;
}

mlc::ErrorTrace::size_type mlc::ErrorTrace::size() const noexcept {
	return m_trace.size();
}

bool mlc::ErrorTrace::empty() const noexcept {
	return m_trace.empty();
}
