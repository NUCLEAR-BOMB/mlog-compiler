#pragma once

#include "Commands.hpp"
#include "Operators.hpp"

#include <fstream>
#include <string>
#include <string_view>

namespace mlc
{
	// Extract command from line. If command successfully extracted from line - true, else - false
	bool extract_command(std::string& line, mlc::Command& outcommand) noexcept;

	bool extract_operator(std::string& line, mlc::Operator& outoperator) noexcept;

	// Check if a command is comment
	bool check_line_comment(std::string& line) noexcept;
}

