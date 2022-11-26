#include "Parsing.hpp"

#include <sstream>
#include <iostream>
#include <regex>
#include <cctype>
#include <iterator>

mlc::ErrorTrace mlc::extract_command(const mlc::Line& line, std::vector<mlc::Command>& outcommand) noexcept
{
	mlc::ErrorTrace errtrace;

	const auto& strline = line.get();
	//std::istringstream linestream{ std::wstring(line) };

	// Remove leading, trailing and extra spaces
	//line = std::regex_replace(line, std::regex("^ +| +$|( ) +"), "$1");

	auto commandArgsStart = strline.find_first_of('(');
	if (commandArgsStart == std::wstring::npos) return { mlc::CriticalError() };

	// Command name
	const std::wstring cmnd{ strline.substr(0, commandArgsStart)};

	const auto commandEnd = strline.find_first_of(')', commandArgsStart);

	// check if there is a closing bracket
	if (commandEnd == std::wstring::npos) return { mlc::CriticalError() };

	std::vector<std::wstring> args; args.reserve(8);

	std::size_t i;
	for (i = commandArgsStart + 1; i < strline.size(); ++i)
	{
		if (strline[i] != ' ') {
			// find start of the argument
			const auto endArg = strline.find_first_of(',', i);

			std::wstring arg;
			if (endArg == std::wstring::npos)
			{
				//auto argstr = line.substr(i, commandEnd - i);
				auto lastArgEnd = strline.find_last_not_of(L" \t", commandEnd);
				arg = strline.substr(i, lastArgEnd - i);
			} else {
				arg = strline.substr(i, endArg - i);
			}

			// check if this the last argument
			errtrace.push(mlc::extract_operator(mlc::Line(arg, line), arg, outcommand));

			args.push_back(std::move(arg));

			if (endArg == std::wstring::npos) break;

			i = endArg + 1;
		}
	}

	// check if there are extra characters after the command
	if (std::find_if_not(strline.begin() + commandEnd + 1, strline.end(), ::isspace) != strline.end()) {
		errtrace.push(mlc::Error(line, L"Extra characters after the command"));
		return errtrace;
	}

	mlc::CommandType cmdtype;
	bool is_command_exist = mlc::find_command_type(cmnd, cmdtype);

	// Check if the command name exists
	if (!is_command_exist) { 
		errtrace.push(mlc::Error(line, L"Unknown command name"));
		return errtrace;
	}

	//if (cmdtype.arg_count() == args.size()) std::;

	outcommand.push_back(mlc::Command(args, cmdtype));
	return errtrace;
}

mlc::Error mlc::extract_operator(const mlc::Line& line, std::wstring& replaced, std::vector<mlc::Command>& outoperator) noexcept
{
	using op = mlc::Operator;
	using arg_t = typename op::argument_type;
	const auto& strline = line.get();

	if (strline.find_first_of(L"()") != std::wstring::npos) return mlc::CriticalError();

	constexpr auto arg_filter = [](wchar_t c) -> bool {
		return std::isalnum(c) || c == '_' || c == '\"' || c == '\'';
	};

	constexpr auto operator_filter = [](wchar_t c) -> bool {
		return mlc::OPERATOR_LIST.find(c) != mlc::OPERATOR_LIST.end();
	};

	const auto firstArgStart = std::find_if(strline.begin(), strline.end(), arg_filter);

	const auto operatorNameStart = std::find_if(firstArgStart, strline.end(), operator_filter);
	const auto operatorNameEnd = std::find_if_not(operatorNameStart, strline.end(), operator_filter);

	const auto firstArgEnd = std::find_if(
		std::make_reverse_iterator(operatorNameStart), std::make_reverse_iterator(firstArgStart), arg_filter
	).base();

	const auto secondArgStart = std::find_if(operatorNameEnd, strline.end(), arg_filter);
	const auto secondArgEnd = std::find_if(strline.rbegin(), std::make_reverse_iterator(secondArgStart), arg_filter).base();

	if (static_cast<std::size_t>(std::distance(operatorNameStart, operatorNameEnd)) > mlc::OperatorType::MAX_NAME_SIZE || operatorNameStart == strline.end() || operatorNameEnd == strline.end()) return mlc::CriticalError();
	if (firstArgStart	  == strline.end() || firstArgEnd	  == strline.end()) return mlc::CriticalError();
	if (secondArgStart	  == strline.end()) return mlc::CriticalError();

	const arg_t					 first = arg_t(firstArgStart, firstArgEnd);
	const typename op::name_type opname = *operatorNameStart;
	const arg_t					 second = arg_t(secondArgStart, secondArgEnd);

	mlc::OperatorType optype;
	bool is_operator_exist = mlc::find_operator_type(opname, optype);

	if (!is_operator_exist) return mlc::Error(line, L"Unknown operator name");

	std::wstring replace_second;
	if (!mlc::extract_operator(mlc::Line(second, line), replace_second, outoperator).critical()) {
		
	}
	else {
		replace_second = second;
	}

	auto commands = mlc::Operator(first, replace_second, optype).convert_to_command();

	replaced = commands.first;
	outoperator.insert(outoperator.end(), commands.second.begin(), commands.second.end());

	return mlc::Error();
}

mlc::Error mlc::extract_operator(const mlc::Line& line, std::vector<mlc::Command>& outoperator) noexcept {
	std::wstring rep;
	return mlc::extract_operator(line, rep, outoperator);
}

void mlc::uncomment_line(mlc::Line& line) noexcept
{
	auto commentPos = line.get().find_first_of('#');
	
	if (commentPos != std::wstring::npos) {
		line.get() = line.get().substr(0, commentPos);
	}
}

