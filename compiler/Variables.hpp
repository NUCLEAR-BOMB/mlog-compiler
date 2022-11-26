#pragma once
#include "Commands.hpp"

#include <string>
#include <set>

namespace mlc
{
	class Variable
	{
	public:

		Variable(const std::wstring_view name) noexcept;
		Variable(const std::wstring& name) noexcept;

		const std::wstring& name() const noexcept;

		// For std::set
		bool operator<(const Variable& right) const noexcept;

	private:
		const std::wstring m_name;
	};

	class VariablesPool
	{
	public:

		// Add a variable to pool
		bool add(const Variable& varname) noexcept;

		// Get the size of pool
		std::size_t size() const noexcept;

		// Check if the variable is in the pool
		bool contains(const mlc::Variable& var) const noexcept;

	private:
		std::set<Variable, std::less<>> m_pool;
	};

	// Check if the variable is variable
	bool is_variable_valid(const mlc::Variable& var) noexcept;

	// Check if all available variables in command valid
	bool is_command_variables_valid(const VariablesPool& pool, const mlc::Command& cmd) noexcept;
}