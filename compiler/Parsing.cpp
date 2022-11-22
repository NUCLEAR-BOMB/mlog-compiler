#include "Parsing.hpp"

#include <sstream>
#include <array>
#include <iostream>
#include <regex>

mlc::Error mlc::extract_command(const mlc::Line& line, mlc::Command& outcommand) noexcept
{
	const auto& strline = line.get();
	//std::istringstream linestream{ std::string(line) };

	// Remove leading, trailing and extra spaces
	//line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

	auto commandArgsStart = strline.find_first_of('(');
	if (commandArgsStart == std::string::npos) return mlc::CriticalError();

	// Command name
	const std::string cmnd{ strline.substr(0, commandArgsStart)};

	std::vector<std::string> args; args.reserve(8);

	const auto commandEnd = strline.find_first_of(')');

	// check if there is a closing bracket
	if (commandEnd == std::string::npos) return mlc::CriticalError();

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
	if (std::find_if_not(strline.begin() + commandEnd + 1, strline.end(), ::isspace) != strline.end()) 
		return mlc::Error(line, "Extra characters after the command");

	mlc::CommandType cmdtype;
	bool is_command_exist = mlc::find_command_type(cmnd, cmdtype);

	// Check if the command name exists
	if (!is_command_exist) return mlc::Error(line, "Unknown command name");

	outcommand = mlc::Command(args, cmdtype);
	return mlc::Error();
}

mlc::Error mlc::extract_operator(const mlc::Line& line, mlc::Operator& outoperator) noexcept
{
	using op = mlc::Operator;
	using arg_t = typename op::argument_type;
	const auto& strline = line.get();

	constexpr auto op_name_chars_filter = [](char c) constexpr {
		return c != '(' && c != ')' && c != '\"' && c != '\'';
	};

	auto firstArgStart = std::find_if(strline.begin(), strline.end(), ::isalnum);
	auto firstArgEnd = std::find_if_not(firstArgStart, strline.end(), ::isalnum);

	if (firstArgStart == strline.end()) return mlc::CriticalError();
	if (firstArgEnd == strline.end()) return mlc::CriticalError();

	auto operatorNameStart = std::find_if(firstArgStart, strline.end(), 
		[&op_name_chars_filter](char c) { return ::ispunct(c) && op_name_chars_filter(c); });

	auto operatorNameEnd = std::find_if(operatorNameStart, strline.end(), 
		[&op_name_chars_filter](char c) { return !::ispunct(c) || !op_name_chars_filter(c); });

	if (std::distance(operatorNameStart, operatorNameEnd) > op::MAX_NAME_SIZE) return mlc::CriticalError();

	auto secondArgStart = std::find_if(operatorNameStart, strline.end(), ::isalnum);
	auto secondArgEnd = std::find_if_not(secondArgStart, strline.end(), ::isalnum);

	if (secondArgStart == strline.end()) return mlc::CriticalError();

	typename op::name_type opname;
	arg_t first, second;
	
	first = arg_t(firstArgStart, firstArgEnd);
	opname = *operatorNameStart;
	second = arg_t(secondArgStart, secondArgEnd);

	mlc::OperatorType optype;
	bool is_operator_exist = mlc::find_operator_type(opname, optype);

	if (!is_operator_exist) return mlc::Error(line, "Unknown operator name");

	outoperator = mlc::Operator(first, second, optype);

	return mlc::Error();
}

void mlc::uncomment_line(mlc::Line& line) noexcept
{
	auto commentPos = line.get().find_first_of('#');
	
	if (commentPos != std::string::npos) {
		line.get() = line.get().substr(0, commentPos);
	}
}

