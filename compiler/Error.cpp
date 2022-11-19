#include "Error.hpp"

#include <sstream>

mlc::Error::Error(const mlc::Line& line, const std::string_view message) noexcept 
	: m_critical(false) {
	std::stringstream ss;
	ss << "Line " << line.line() << " [Error] | " << line.get() << " \\ " << message;
	m_str = ss.str();
}

mlc::Error::Error(const mlc::Line& line) noexcept
	: m_critical(false) {
	std::stringstream ss;
	ss << "Line " << line.line() << " [Error] | " << line.get();
	m_str = ss.str();
}

mlc::Error::Error(const CriticalError&) noexcept 
	: m_critical(true), m_str()
{}

mlc::Error::Error() noexcept
	: m_str(), m_critical(false)
{}

mlc::Error::operator bool() const noexcept {
	return this->empty();
}

bool mlc::Error::empty() const noexcept {
	return m_critical || m_str.empty();
}

const std::string& mlc::Error::str() const noexcept {
	return m_str;
}

void mlc::Error::clear() noexcept {
	m_str.clear();
}

bool mlc::Error::critical() const noexcept {
	return m_critical;
}

std::ostream& mlc::operator<<(std::ostream& os, const Error& err) noexcept {
	os << err.m_str;
	return os;
}

std::ostream& mlc::operator<<(std::ostream& os, const ErrorTrace& errtrace) noexcept 
{
	if (errtrace.empty()) return os;

	auto it = errtrace.m_trace.cbegin();
	for (;it != std::prev(errtrace.m_trace.cend()); ++it) {
		os << *it << '\n';
	}
	os << *it;

	return os;
}

mlc::ErrorTrace::ErrorTrace() noexcept
	: m_trace()
{}

mlc::ErrorTrace::ErrorTrace(std::initializer_list<mlc::Error> list) noexcept
	: m_trace(list)
{}

const mlc::Error& mlc::ErrorTrace::push(const mlc::Error& error) noexcept {
	if (error.empty()) return error;
	m_trace.push_back(error);
	return error;
}

const mlc::ErrorTrace& mlc::ErrorTrace::push(const ErrorTrace& trace) noexcept {
	m_trace.insert(trace.m_trace.begin(), trace.m_trace.cbegin(), trace.m_trace.cend());
	return trace;
}

mlc::Error mlc::ErrorTrace::pop() noexcept {
	mlc::Error tmp = m_trace.back();
	m_trace.pop_back();
	return tmp;
}

mlc::ErrorTrace::size_type mlc::ErrorTrace::size() const noexcept {
	return m_trace.size();
}

bool mlc::ErrorTrace::empty() const noexcept {
	return m_trace.empty();
}

mlc::ErrorTrace::operator bool() const noexcept {
	return !this->empty();
}

mlc::ErrorTrace::container_type& mlc::ErrorTrace::container() noexcept {
	return m_trace;
}

const mlc::ErrorTrace::container_type& mlc::ErrorTrace::container() const noexcept {
	return m_trace;
}

void mlc::ErrorTrace::clear() noexcept {
	m_trace.clear();
}
