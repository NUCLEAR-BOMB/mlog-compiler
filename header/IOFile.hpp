#pragma once

#include "Line.hpp"

#include <fstream>

namespace mlc
{

class InputFile
{
public:

	using stream_type = std::ifstream;
	using line_type = mlc::Line;

	InputFile(mlc::string_const_ref_t file_name) noexcept;

	~InputFile() noexcept;

	line_type read_next_line() noexcept;

	bool is_eof() const noexcept;

	stream_type& stream() noexcept;

private:
	stream_type m_stream;
	line_type::line_number_type m_linenum;
};

class OutputFile
{
public:

	using stream_type = std::ofstream;

	OutputFile(mlc::string_const_ref_t file_name) noexcept;

	~OutputFile() noexcept;

	void write_next_line(mlc::string_const_ref_t str) noexcept;

	stream_type& stream() noexcept;

private:
	stream_type m_stream;
};

}
