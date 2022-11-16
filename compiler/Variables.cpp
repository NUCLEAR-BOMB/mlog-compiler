#include "Variables.hpp"

#include <algorithm>
#include <string_view>
#include <regex>
#include <regex>

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

mlc::Variable::Variable(const std::string_view name) noexcept
	: m_name(name)
{}

mlc::Variable::Variable(const std::string& name) noexcept
	: m_name(name)
{}

bool mlc::Variable::operator<(const Variable& right) const noexcept {
	return m_name < right.m_name;
}

bool mlc::is_variable_valid(const mlc::Variable& var) noexcept {
	//std::string varname = std::regex_replace(cmd[0], std::regex("^ +| +$"), "");
	const std::string_view varname = var.name();

	bool only_digits = std::all_of(varname.begin(), varname.end(), ::isdigit);

	bool quoted_text = ((varname.front() == '\"' && varname.back() == '\"') ||
		(varname.front() == '\'' && varname.back() == '\''));

	return !only_digits && !quoted_text;;
}

bool mlc::is_command_variables_valid(const VariablesPool& pool, const mlc::Command& cmd) noexcept
{
	const auto& args = cmd.args();
	const auto& ignore_args = cmd.ignore_args();
	for (std::size_t i = 0; i < args.size(); ++i)
	{
		if (mlc::is_creating_var(cmd) && (i == static_cast<std::size_t>(cmd.out_arg_index()))) continue;
		if (std::find(ignore_args.cbegin(), ignore_args.cend(), i) != ignore_args.cend()) continue;

		if (!mlc::is_variable_valid(args[i])) continue;
		if (!pool.contains(args[i])) return false;
	}
	return true;
}
