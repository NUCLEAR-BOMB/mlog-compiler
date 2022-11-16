#include "Parsing.hpp"

#include <sstream>
#include <array>
#include <iostream>
#include <regex>

bool mlc::get_command_type(const std::string_view cmdname, mlc::CommandType& outtype) noexcept
{
	// check if COMMAND_LIST contains command.type() 
	//auto t = mlc::COMMAND_LIST.find(mlc::CommandType(cmdname));
	auto t = mlc::COMMAND_LIST.find(mlc::CommandType(cmdname));

	if (t != mlc::COMMAND_LIST.cend()) {
		outtype = *t;
		return true;
	}
	return false;
}

bool mlc::get_command_type(const std::string_view cmdname) noexcept {
	return mlc::COMMAND_LIST.find(mlc::CommandType(cmdname)) != mlc::COMMAND_LIST.cend();
}

bool mlc::extract_command(std::string& line, mlc::Command& outcommand) noexcept
{
	std::istringstream linestream{ std::string(line) };

	// Remove leading, trailing and extra spaces
	//line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

	auto commandArgsStart = line.find_first_of('(');
	if (commandArgsStart == std::string::npos) return false;

	// Command name
	const std::string cmnd{line.substr(0, commandArgsStart)};

	std::vector<std::string> args; args.reserve(8);

	const auto commandEnd = line.find_first_of(')');

	// check if there is a closing bracket
	if (commandEnd == std::string::npos) return false;

	std::size_t i;
	for (i = commandArgsStart + 1; i < line.size(); ++i) 
	{
		if (line[i] != ' ') {
			// find start of the argument
			auto endArg = line.find_first_of(',', i);

			// check if this the last argument
			if (endArg == std::string::npos) 
			{
				//auto argstr = line.substr(i, commandEnd - i);
				auto lastArgEnd = line.find_last_not_of(" \t", commandEnd);
				args.emplace_back(line.substr(i, lastArgEnd - i));
				break;
			} else {
				args.emplace_back(line.substr(i, endArg - i));
			}
			i = endArg + 1;
		}
	}

	// check if there are extra characters after the command
	if (std::find_if_not(line.begin() + commandEnd + 1, line.end(), ::isspace) != line.end()) return false;

	mlc::CommandType cmdtype;
	bool is_command_exist = mlc::get_command_type(cmnd, cmdtype);

	// Check if the command name exists
	if (!is_command_exist) return false;

	outcommand = mlc::Command(args, cmdtype);
	return true;
}

bool mlc::check_line_comment(std::string& line) noexcept
{
	auto commentPos = line.find_first_of('#');
	
	if (commentPos != std::string::npos) {
		line = line.substr(0, commentPos);
		return true;
	}

	return false;
}

