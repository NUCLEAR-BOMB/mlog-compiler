#include "IOFile.hpp"

#include "ErrorLogger.hpp"

#include <utility>

mlc::InputFile::InputFile(mlc::string_const_ref_t file_name) noexcept
	: m_stream(file_name.data()), m_linenum(0)
{
	if (!m_stream.is_open()) {
		mlc::msglog("Input file open error");
	}
}

mlc::InputFile::~InputFile() noexcept {
	m_stream.close();
}

mlc::InputFile::line_type mlc::InputFile::read_next_line() noexcept
{
	++m_linenum;

	mlc::string_t line;
	std::getline(m_stream, line);

	return mlc::Line(m_linenum, std::move(line));
}

bool mlc::InputFile::is_eof() const noexcept
{
	return m_stream.eof();
}

mlc::InputFile::stream_type& mlc::InputFile::stream() noexcept {
	return m_stream;
}

mlc::OutputFile::OutputFile(mlc::string_const_ref_t file_name) noexcept
	: m_stream(file_name.data())
{
	if (!m_stream.is_open()) {
		mlc::msglog("Output file open error");
	}
}

mlc::OutputFile::~OutputFile() noexcept {
	m_stream.close();
}

void mlc::OutputFile::write_next_line(mlc::string_const_ref_t str) noexcept
{
	m_stream << str;
}

mlc::OutputFile::stream_type& mlc::OutputFile::stream() noexcept {
	return m_stream;
}
