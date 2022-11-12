#include "Parsing.hpp"

#include <string>
#include <string_view>
#include <sstream>
#include <array>

namespace {
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

mlc::Command mlc::extract_command(const std::string_view line)
{
	std::istringstream linestream{ std::string(line) };

	auto commandArgsStart = line.find_first_of('(');
	std::string cmnd{line.substr(0, commandArgsStart)};

	std::vector<std::string> args; args.reserve(8);

	std::string argstr;
	for (std::size_t i = commandArgsStart + 1; i < line.size(); ++i) {
		auto c = line[i];
		if (c != ' ') {
			auto endArg = line.find_first_of(',', i);

			if (endArg == std::string::npos) {
				auto s = line.substr(i, line.size() - i - 1);
				args.emplace_back(s); break;
			} else {
				args.emplace_back(line.substr(i, endArg - i));
			}

			i = endArg;
		}
	}

	return mlc::Command(cmnd, args);
}

