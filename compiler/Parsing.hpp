#pragma once

#include "Commands.hpp"
#include <fstream>

namespace mlc
{
	// Check if a command exists
	bool verify_command(const mlc::Command& command) noexcept;

	// Extract command from line. If command successfully extracted from line - true, else - false
	bool extract_command(std::string& line, mlc::Command& outcommand) noexcept;

	// Check if a command is comment
	bool check_line_comment(std::string& line) noexcept;
}

