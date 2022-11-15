#include "Variables.hpp"

#include <algorithm>
#include <string_view>
#include <regex>
#include <regex>

namespace {

	bool is_command_args_valid(const mlc::Command& cmd) noexcept 
	{
		//std::string varname = std::regex_replace(cmd[0], std::regex("^ +| +$"), "");
		const std::string_view varname = cmd[cmd.out_arg()];

		bool only_digits = std::all_of(varname.begin(), varname.end(), ::isdigit);

		bool quoted_text = ((varname.front() == '\"' && varname.back() == '\"') || 
						    (varname.front() == '\'' && varname.back() == '\''));

		return !only_digits && !quoted_text;
	}

}

bool mlc::VariablesPool::add(const mlc::Command& cmd) noexcept 
{
	if (!is_command_args_valid(cmd)) return false;

	m_pool.insert(cmd.name());
	return true;
}

std::size_t mlc::VariablesPool::size() const noexcept {
	return m_pool.size();
}

bool mlc::VariablesPool::contains(const mlc::Variable& var) const noexcept {
	return m_pool.find(var) != m_pool.cend();
}

bool mlc::VariablesPool::is_creating_var(const mlc::CommandType& cmd) noexcept {
	return cmd.out_arg() != cmd.NO_OUT_ARG;
}

bool mlc::Variable::operator<(const Variable& right) const noexcept {
	return m_name < right.m_name;
}
