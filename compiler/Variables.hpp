#pragma once
#include "Commands.hpp"

#include <string>
#include <set>

namespace mlc
{
	class Variable
	{
	public:

		Variable(const std::string_view name) noexcept;
		Variable(const std::string& name) noexcept;

		const std::string& name() const noexcept { return m_name; }

		bool operator<(const Variable& right) const noexcept;

	private:
		const std::string m_name;
	};

	class VariablesPool
	{
	public:

		bool add(const Variable& varname) noexcept;

		std::size_t size() const noexcept;

		bool contains(const mlc::Variable& var) const noexcept;

	private:
		std::set<Variable, std::less<>> m_pool;
	};

	bool is_variable_valid(const mlc::Variable& var) noexcept;

	// Check if command type creating var
	bool is_creating_var(const mlc::CommandType& cmdtype) noexcept;

	bool is_command_variables_valid(const VariablesPool& pool, const mlc::Command& cmd) noexcept;
}