#include "Parsing.hpp"

#include <string>
#include <string_view>
#include <sstream>
#include <array>
#include <iostream>
#include <regex>

namespace {
	// Table of some mlog commands
	static const std::array<mlc::CommandType, 11> COMMAND_LIST({
		mlc::CommandType(std::string("read"), 3),
		mlc::CommandType(std::string("write"), 3),
		mlc::CommandType(std::string("draw"), 7),
		mlc::CommandType(std::string("print"), 1),
		mlc::CommandType(std::string("drawflush"), 1),
		mlc::CommandType(std::string("printflush"), 1),
		mlc::CommandType(std::string("getlink"), 2),
		mlc::CommandType(std::string("control"), 6),
		mlc::CommandType(std::string("radar"), 7),
		mlc::CommandType(std::string("sensor"), 3),
		mlc::CommandType(std::string("set"), 2),
	});
}

bool mlc::verify_command(const mlc::Command& command) noexcept {
	for (const auto& i : COMMAND_LIST) {
		if (i == command.type()) return true;
	}
	return false;
}

bool mlc::extract_command(std::string& line, mlc::Command& outcommand) noexcept
{
	std::istringstream linestream{ std::string(line) };

	// Remove leading, trailing and extra spaces
	line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

	auto commandArgsStart = line.find_first_of('(');
	if (commandArgsStart == std::string::npos) return false;

	// Command name
	std::string cmnd{line.substr(0, commandArgsStart)};

	std::vector<std::string> args; args.reserve(8);

	std::string argstr;
	std::size_t i;
	for (i = commandArgsStart + 1; i < line.size(); ++i) 
	{
		if (line[i] != ' ') {
			// find start of the argument
			auto endArg = line.find_first_of(',', i);

			// check if this the last argument
			if (endArg == std::string::npos) {
				args.emplace_back(line.substr(i, line.find_first_of(" )", i) - i));
				break;
			} else {
				args.emplace_back(line.substr(i, endArg - i));
			}
			i = endArg + 1;
		}
	}

	auto commandEnd = line.find_first_of(')', i);
	// check if there is a closing bracket
	if (commandEnd == std::string::npos) return false;

	// check if there are extra characters after the command
	for (std::size_t j = commandEnd + 1; j < line.size(); ++j) {
		if (line[j] != ' ') return false;
	}

	outcommand = mlc::Command(cmnd, args);
	return true;
}

