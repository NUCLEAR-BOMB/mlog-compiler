#pragma once

#include <string>
#include <vector>

namespace mlc
{
	using raw_mlog_command_type = std::string;

	class CommandType
	{
	public:
		using arguments_count_type = std::size_t;

		CommandType(const std::string& command, arguments_count_type arguments_count) noexcept
			: m_command(command), m_arguments_count(arguments_count) {}

		CommandType(const CommandType&) noexcept 
			: m_command(""), m_arguments_count(0) {}

		const std::string& name() const & noexcept { return m_command; }
		arguments_count_type arg_count() const noexcept { return m_arguments_count; }

		bool operator==(const CommandType& right) const noexcept {
			return name() == right.name() && m_arguments_count == right.m_arguments_count;
		}

	protected:
		std::string m_command;
		arguments_count_type m_arguments_count;
	};

	class Command
	{
	public:
		using args_type = std::vector<std::string>;
		using arguments_count_type = std::size_t;

		Command() noexcept 
			: m_command(), m_args() {}
		Command(const std::string& command, const args_type& args) noexcept 
			: m_command(command), m_args(args) {}

		const std::string& name() const& noexcept { return m_command; }
		arguments_count_type arg_count() const noexcept { return m_args.size(); }
		const args_type& args() const& noexcept { return m_args; }
		CommandType type() const noexcept { return CommandType(m_command, m_args.size()); }

		raw_mlog_command_type convert() const noexcept {
			using namespace std::string_literals;
			raw_mlog_command_type out = m_command;
			for (const auto& s : m_args) {
				out += " "s;
				out += s;
			}
			return out;
		}

	private:
		args_type m_args;
		std::string m_command;
	};

}
