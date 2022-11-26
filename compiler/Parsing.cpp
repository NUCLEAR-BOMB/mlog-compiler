#include "Parsing.hpp"

#include <sstream>
#include <iostream>
#include <regex>

mlc::ErrorTrace mlc::extract_command(const mlc::Line& line, std::vector<mlc::Command>& outcommand) noexcept
{
	mlc::ErrorTrace errtrace;

	const auto& strline = line.get();
	//std::istringstream linestream{ std::string(line) };

	// Remove leading, trailing and extra spaces
	//line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

	auto commandArgsStart = strline.find_first_of('(');
	if (commandArgsStart == std::string::npos) return { mlc::CriticalError() };

	// Command name
	const std::string cmnd{ strline.substr(0, commandArgsStart)};

	const auto commandEnd = strline.find_first_of(')', commandArgsStart);

	// check if there is a closing bracket
	if (commandEnd == std::string::npos) return { mlc::CriticalError() };

	std::vector<std::string> args; args.reserve(8);

	std::size_t i;
	for (i = commandArgsStart + 1; i < strline.size(); ++i)
	{
		if (strline[i] != ' ') {
			// find start of the argument
			const auto endArg = strline.find_first_of(',', i);

			std::string arg;
			if (endArg == std::string::npos)
			{
				//auto argstr = line.substr(i, commandEnd - i);
				auto lastArgEnd = strline.find_last_not_of(" \t", commandEnd);
				arg = strline.substr(i, lastArgEnd - i);
			} else {
				arg = strline.substr(i, endArg - i);
			}

			// check if this the last argument
			errtrace.push(mlc::extract_operator(mlc::Line(arg, line), arg, outcommand));

			args.push_back(std::move(arg));

			if (endArg == std::string::npos) break;

			i = endArg + 1;
		}
	}

	// check if there are extra characters after the command
	if (std::find_if_not(strline.begin() + commandEnd + 1, strline.end(), ::isspace) != strline.end()) {
		errtrace.push(mlc::Error(line, "Extra characters after the command"));
		return errtrace;
	}

	mlc::CommandType cmdtype;
	bool is_command_exist = mlc::find_command_type(cmnd, cmdtype);

	// Check if the command name exists
	if (!is_command_exist) { 
		errtrace.push(mlc::Error(line, "Unknown command name"));
		return errtrace;
	}

	//if (cmdtype.arg_count() == args.size()) std::;

	outcommand.push_back(mlc::Command(args, cmdtype));
	return errtrace;
}

mlc::Error mlc::extract_operator(const mlc::Line& line, std::string& replaced, std::vector<mlc::Command>& outoperator) noexcept
{
	using op = mlc::Operator;
	using arg_t = typename op::argument_type;
	const auto& strline = line.get();

	constexpr auto op_name_chars_filter = [](char c) constexpr {
		return c != '(' && c != ')' && c != '\"' && c != '\'';
	};

	const auto firstArgStart = std::find_if(strline.begin(), strline.end(), ::isalnum);
	const auto firstArgEnd = std::find_if_not(firstArgStart, strline.end(), ::isalnum);

	if (firstArgStart == strline.end()) return mlc::CriticalError();
	if (firstArgEnd == strline.end()) return mlc::CriticalError();

	const auto operatorNameStart = std::find_if(firstArgStart, strline.end(), 
		[&op_name_chars_filter](char c) { return ::ispunct(c) && op_name_chars_filter(c); });

	const auto operatorNameEnd = std::find_if(operatorNameStart, strline.end(), 
		[&op_name_chars_filter](char c) { return !::ispunct(c) || !op_name_chars_filter(c); });

	if (static_cast<std::size_t>(std::distance(operatorNameStart, operatorNameEnd)) > op::MAX_NAME_SIZE) return mlc::CriticalError();
	if (std::find_if_not(firstArgStart, operatorNameStart, op_name_chars_filter) != operatorNameStart) return mlc::CriticalError();

	const auto secondArgStart = std::find_if(operatorNameStart, strline.end(), ::isalnum);
	const auto secondArgEnd = std::find_if_not(secondArgStart, strline.end(), ::isalnum);

	if (secondArgStart == strline.end()) return mlc::CriticalError();

	typename op::name_type opname;
	arg_t first, second;
	
	first = arg_t(firstArgStart, firstArgEnd);
	opname = *operatorNameStart;
	second = arg_t(secondArgStart, secondArgEnd);

	mlc::OperatorType optype;
	bool is_operator_exist = mlc::find_operator_type(opname, optype);

	if (!is_operator_exist) return mlc::Error(line, "Unknown operator name");

	auto commands = mlc::Operator(first, second, optype).convert_to_command();

	replaced = commands.first;
	outoperator.insert(outoperator.begin(), commands.second.begin(), commands.second.end());

	return mlc::Error();
}

mlc::Error mlc::extract_operator(const mlc::Line& line, std::vector<mlc::Command>& outoperator) noexcept {
	std::string rep;
	return mlc::extract_operator(line, rep, outoperator);
}

void mlc::uncomment_line(mlc::Line& line) noexcept
{
	auto commentPos = line.get().find_first_of('#');
	
	if (commentPos != std::string::npos) {
		line.get() = line.get().substr(0, commentPos);
	}
}

