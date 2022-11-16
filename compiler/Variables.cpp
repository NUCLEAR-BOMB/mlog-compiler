#include "Variables.hpp"

#include <algorithm>
#include <string_view>
#include <regex>
#include <regex>

namespace {

	bool is_variable_valid(const mlc::Variable& cmd) noexcept 
	{
		//std::string varname = std::regex_replace(cmd[0], std::regex("^ +| +$"), "");
		const std::string_view varname = cmd.name();

		bool only_digits = std::all_of(varname.begin(), varname.end(), ::isdigit);

		bool quoted_text = ((varname.front() == '\"' && varname.back() == '\"') || 
						    (varname.front() == '\'' && varname.back() == '\''));

		return !only_digits && !quoted_text;
	}

}

bool mlc::VariablesPool::add(const Variable& varname) noexcept
{
	if (!is_variable_valid(varname)) return false;

	m_pool.insert(varname.name());
	return true;
}

std::size_t mlc::VariablesPool::size() const noexcept {
	return m_pool.size();
}

bool mlc::VariablesPool::contains(const mlc::Variable& var) const noexcept {
	return m_pool.find(var) != m_pool.cend();
}

bool mlc::Variable::operator<(const Variable& right) const noexcept {
	return m_name < right.m_name;
}

bool mlc::is_creating_var(const mlc::CommandType& cmd) noexcept {
	return cmd.out_arg_index() != cmd.NO_OUT_ARG;
}
