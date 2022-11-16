#pragma once
#include "Commands.hpp"

#include <string>
#include <set>

namespace mlc
{
	class Variable
	{
	public:

		Variable(const std::string& name) noexcept
			: m_name(name) {}

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

	bool is_creating_var(const mlc::CommandType& cmd) noexcept;
	bool is_variable(const std::string_view varname) noexcept;
}