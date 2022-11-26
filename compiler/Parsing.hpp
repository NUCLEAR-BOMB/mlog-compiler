#pragma once

#include "Commands.hpp"
#include "Operators.hpp"
#include "Line.hpp"
#include "Error.hpp"

#include <fstream>
#include <string>
#include <string_view>
#include <vector>

namespace mlc
{
	// Extract command from line. If command successfully extracted from line - true, else - false
	mlc::ErrorTrace extract_command(const mlc::Line& line, std::vector<mlc::Command>& outcommand) noexcept;

	mlc::Error extract_operator(const mlc::Line& line, std::wstring& replaced, std::vector<mlc::Command>& outoperator) noexcept;
	mlc::Error extract_operator(const mlc::Line& line, std::vector<mlc::Command>& outoperator) noexcept;

	void uncomment_line(mlc::Line& line) noexcept;
}

