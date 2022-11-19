#pragma once

#include "Commands.hpp"
#include "Operators.hpp"
#include "Line.hpp"
#include "Error.hpp"

#include <fstream>
#include <string>
#include <string_view>

namespace mlc
{
	// Extract command from line. If command successfully extracted from line - true, else - false
	mlc::Error extract_command(const mlc::Line& line, mlc::Command& outcommand) noexcept;

	mlc::Error extract_operator(const mlc::Line& line, mlc::Operator& outoperator) noexcept;

	void uncomment_line(mlc::Line& line) noexcept;
}

