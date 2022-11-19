#include "Parsing.hpp"

#include <sstream>
#include <array>
#include <iostream>
#include <regex>

bool mlc::extract_command(const mlc::Line& line, mlc::Command& outcommand) noexcept
{
	const auto& strline = line.get();
	//std::istringstream linestream{ std::string(line) };

	// Remove leading, trailing and extra spaces
	//line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

	auto commandArgsStart = strline.find_first_of('(');
	if (commandArgsStart == std::string::npos) return false;

	// Command name
	const std::string cmnd{ strline.substr(0, commandArgsStart)};

	std::vector<std::string> args; args.reserve(8);

	const auto commandEnd = strline.find_first_of(')');

	// check if there is a closing bracket
	if (commandEnd == std::string::npos) return false;

	std::size_t i;
	for (i = commandArgsStart + 1; i < strline.size(); ++i)
	{
		if (strline[i] != ' ') {
			// find start of the argument
			auto endArg = strline.find_first_of(',', i);

			// check if this the last argument
			if (endArg == std::string::npos) 
			{
				//auto argstr = line.substr(i, commandEnd - i);
				auto lastArgEnd = strline.find_last_not_of(" \t", commandEnd);
				args.emplace_back(strline.substr(i, lastArgEnd - i));
				break;
			} else {
				args.emplace_back(strline.substr(i, endArg - i));
			}
			i = endArg + 1;
		}
	}

	// check if there are extra characters after the command
	if (std::find_if_not(strline.begin() + commandEnd + 1, strline.end(), ::isspace) != strline.end()) return false;

	mlc::CommandType cmdtype;
	bool is_command_exist = mlc::find_command_type(cmnd, cmdtype);

	// Check if the command name exists
	if (!is_command_exist) return false;

	outcommand = mlc::Command(args, cmdtype);
	return true;
}

bool mlc::extract_operator(const mlc::Line& line, mlc::Operator& outoperator) noexcept
{
	using op = mlc::Operator;
	const auto& strline = line.get();

	auto firstArgStart = strline.find_first_not_of(" \t");
	auto firstArgEnd = strline.find_first_of(" \n", firstArgStart);
	if (firstArgEnd == std::string::npos) return false;

	auto operatorNameStart = strline.find_first_not_of(" \t", firstArgEnd + 1);
	auto operatorNameEnd = strline.find_first_of(" \t", operatorNameStart + 1);
	if (operatorNameStart != operatorNameEnd - 1) return false;

	auto secondArgStart = strline.find_first_not_of(" \t", operatorNameEnd + 1);
	auto secondArgEnd = strline.find_first_of(" \t", secondArgStart + 1) - 1;

	typename op::name_type opname;
	typename op::argument_type first, second;
	
	first = strline.substr(firstArgStart, firstArgEnd - firstArgStart);
	opname = strline[operatorNameStart];
	second = strline.substr(secondArgStart, secondArgEnd - secondArgStart);

	mlc::OperatorType optype;
	bool is_operator_exist = mlc::find_operator_type(opname, optype);

	if (!is_operator_exist) return false;

	outoperator = mlc::Operator(first, second, optype);

	return true;
}

void mlc::uncomment_line(mlc::Line& line) noexcept
{
	auto commentPos = line.get().find_first_of('#');
	
	if (commentPos != std::string::npos) {
		line.get() = line.get().substr(0, commentPos);
	}
}

