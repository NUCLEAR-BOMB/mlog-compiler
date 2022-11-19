#pragma once

#include "Commands.hpp"
#include "Operators.hpp"
#include "Line.hpp"

#include <fstream>
#include <string>
#include <string_view>

namespace mlc
{
	// Extract command from line. If command successfully extracted from line - true, else - false
	bool extract_command(const mlc::Line& line, mlc::Command& outcommand) noexcept;

	bool extract_operator(const mlc::Line& line, mlc::Operator& outoperator) noexcept;

	void uncomment_line(mlc::Line& line) noexcept;
}

