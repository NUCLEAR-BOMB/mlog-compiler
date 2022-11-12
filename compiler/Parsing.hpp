#pragma once

#include "Commands.hpp"
#include <fstream>

namespace mlc
{
	bool	     verify_command(const mlc::Command& command) noexcept;
	mlc::Command extract_command(const std::string_view line);
}

