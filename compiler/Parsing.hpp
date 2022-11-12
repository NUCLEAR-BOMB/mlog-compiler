#pragma once

#include "Commands.hpp"
#include <fstream>

namespace mlc
{
	void read_file(std::fstream& file) noexcept;

	bool	     verify_command(const mlc::Command& command) noexcept;
	mlc::Command extract_command(const std::string_view line);
}

