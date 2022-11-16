#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <array>

namespace mlc
{
	using raw_mlog_command_type = std::string;

	class Command;

	class CommandType
	{
	public:
		using arguments_count_type = std::size_t;
		using out_arg_type = int;
		using ignore_args_type = std::vector<unsigned short>;

		static constexpr out_arg_type NO_OUT_ARG = -1;

		CommandType() noexcept;
		CommandType(
			const std::string_view command, 
			arguments_count_type arguments_count = ~std::size_t(0), 
			out_arg_type out_arg_				 = NO_OUT_ARG, 
			const ignore_args_type& ignore_args	 = ignore_args_type()
		) noexcept;

		//CommandType(const CommandType&) noexcept = default;

		CommandType(const Command& cmd) noexcept;

		const std::string& name() const& noexcept;
		arguments_count_type arg_count() const noexcept;
		out_arg_type out_arg_index() const noexcept;
		const ignore_args_type& ignore_args() const noexcept;

		bool operator==(const CommandType& right) const noexcept;
		bool operator<(const CommandType& right) const noexcept;

	protected:
		std::string m_command;
		arguments_count_type m_arguments_count;
		out_arg_type m_out_arg;
		ignore_args_type m_ignore_args;
	};

	class Command : public CommandType
	{
		using Base = CommandType;

		//friend bool extract_command(std::string&, mlc::Command&) noexcept;

		Command(const CommandType& type) noexcept;
	public:
		using args_type = std::vector<std::string>;
		using argument_type = std::string;
		using arguments_count_type = typename Base::arguments_count_type;
		using out_arg_type = typename Base::out_arg_type;

		Command() noexcept;
		Command(const std::string& command, const args_type& args, out_arg_type out_arg_index = Base::NO_OUT_ARG) noexcept;
		Command(const args_type& args, const CommandType& cmdtype) noexcept;

		const args_type& args() const noexcept;

		CommandType type() const noexcept;
		const argument_type& out_arg() const noexcept;

		raw_mlog_command_type convert() const noexcept;

		const argument_type& operator[](std::size_t index) const;
		
	protected:
		args_type m_args;
	};

	// Table of some mlog commands
	const std::set<mlc::CommandType> COMMAND_LIST({
		//				 <name>	<arg count> <out arg>
		mlc::CommandType("read",		3,	0,	{}),
		mlc::CommandType("write",		3,	-1, {}),
		mlc::CommandType("draw",		7,	-1, {0}),
		mlc::CommandType("print",		1,	-1, {}),
		mlc::CommandType("drawflush",	1,	-1, {0}),
		mlc::CommandType("printflush",	1,	-1, {0}),
		mlc::CommandType("getlink",		2,	0,	{1}),
		mlc::CommandType("control",		6,	-1, {}),
		mlc::CommandType("radar",		7,	6,	{}),
		mlc::CommandType("sensor",		3,	0,	{}),
		mlc::CommandType("set",			2,	0,	{}),
	});

}
