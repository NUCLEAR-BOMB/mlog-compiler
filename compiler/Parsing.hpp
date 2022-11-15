#pragma once

#include "Commands.hpp"
#include <fstream>
#include <string>
#include <string_view>

namespace mlc
{
	// Check if a command exists and get command type
	bool get_command_type(const std::string_view cmdname, mlc::CommandType& outtype) noexcept;
	bool get_command_type(const std::string_view cmdname) noexcept;

	// Extract command from line. If command successfully extracted from line - true, else - false
	bool extract_command(std::string& line, mlc::Command& outcommand) noexcept;

	// Check if a command is comment
	bool check_line_comment(std::string& line) noexcept;
}

