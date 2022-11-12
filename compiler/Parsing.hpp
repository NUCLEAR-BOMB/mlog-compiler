#pragma once

#include "Commands.hpp"
#include <fstream>

namespace mlc
{
	bool verify_command(const mlc::Command& command) noexcept;
	bool extract_command(std::string& line, mlc::Command& outcommand) noexcept;
}

