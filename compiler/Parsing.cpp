#include "Parsing.hpp"

#include <sstream>
#include <array>
#include <iostream>
#include <regex>

bool mlc::extract_command(std::string& line, mlc::Command& outcommand) noexcept
{
	//std::istringstream linestream{ std::string(line) };

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
	bool is_command_exist = mlc::find_command_type(cmnd, cmdtype);

	// Check if the command name exists
	if (!is_command_exist) return false;

	outcommand = mlc::Command(args, cmdtype);
	return true;
}

bool mlc::extract_operator(std::string& line, mlc::Operator& outoperator) noexcept
{
	using op = mlc::Operator;

	auto firstArgStart = line.find_first_not_of(" \t");
	auto firstArgEnd = line.find_first_of(" \n", firstArgStart);
	if (firstArgEnd == std::string::npos) return false;

	auto operatorNameStart = line.find_first_not_of(" \t", firstArgEnd + 1);
	auto operatorNameEnd = line.find_first_of(" \t", operatorNameStart + 1);
	if (operatorNameStart != operatorNameEnd - 1) return false;

	auto secondArgStart = line.find_first_not_of(" \t", operatorNameEnd + 1);
	auto secondArgEnd = line.find_first_of(" \t", secondArgStart + 1) - 1;

	typename op::name_type opname;
	typename op::argument_type first, second;
	
	first = line.substr(firstArgStart, firstArgEnd - firstArgStart);
	opname = line[operatorNameStart];
	second = line.substr(secondArgStart, secondArgEnd - secondArgStart);

	mlc::OperatorType optype;
	bool is_operator_exist = mlc::find_operator_type(opname, optype);

	if (!is_operator_exist) return false;

	outoperator = mlc::Operator(first, second, optype);

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

