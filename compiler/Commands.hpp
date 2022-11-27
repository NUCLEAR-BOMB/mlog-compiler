#pragma once

#include "Line.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <array>

namespace mlc
{
	using raw_mlog_command_type = std::wstring;

	class Command;

	class CommandType
	{
	public:
		using arguments_count_type = std::size_t;
		using out_arg_type = std::vector<unsigned int>;
		using ignore_args_type = std::vector<unsigned short>;

		CommandType() noexcept;

		// Main constructor
		CommandType(
			const std::wstring_view command, 
			arguments_count_type arguments_count = ~std::size_t(0), 
			out_arg_type out_arg_				 = {},
			const ignore_args_type& ignore_args	 = ignore_args_type()
		) noexcept;

		//CommandType(const CommandType&) noexcept = default;

		// Implicit convertion from mlc::Command
		CommandType(const Command& cmd) noexcept;

		const std::wstring& name() const& noexcept;
		arguments_count_type arg_count() const noexcept;
		out_arg_type out_arg_index() const noexcept;
		const ignore_args_type& ignore_args() const noexcept;

		bool has_out_args() const noexcept;

		bool operator==(const CommandType& right) const noexcept;
		// For std::set
		bool operator<(const CommandType& right) const noexcept;

	protected:
		std::wstring			m_command;
		arguments_count_type	m_arguments_count;
		out_arg_type			m_out_arg;
		bool					m_has_out_args;
		ignore_args_type		m_ignore_args;
	};

	class Command : public CommandType
	{
		using Base = CommandType;

		//friend bool extract_command(std::wstring&, mlc::Command&) noexcept;

		Command(const CommandType& type) noexcept;
	public:
		using args_type = std::vector<std::wstring>;
		using argument_type = std::wstring;
		using arguments_count_type = typename Base::arguments_count_type;
		using out_arg_type = typename Base::out_arg_type;

		Command() noexcept;
		Command(const std::wstring& command, const args_type& args, out_arg_type out_arg_index = {}) noexcept;
		Command(const args_type& args, const CommandType& cmdtype) noexcept;

		const args_type& args() const noexcept;

		// Explicit return mlc::CommandType
		CommandType type() const noexcept;

		// Convert to raw mlog command
		raw_mlog_command_type convert() const noexcept;

		const argument_type& operator[](std::size_t index) const;
		
	protected:
		args_type m_args;
	};

	// Check if the command type creating variable
	bool is_creating_var(const mlc::CommandType& cmdtype) noexcept;

	std::wstring create_temp_variable_name(mlc::Line::line_counter_type l) noexcept;

	// Table of some mlog commands
	const std::set<mlc::CommandType> COMMAND_LIST({
		//				 <name>	<arg count> <out arg> <ignored args>
		mlc::CommandType(L"read",		3,	{0},	{}),
		mlc::CommandType(L"write",		3,	{},		{}),
		mlc::CommandType(L"draw",		7,	{},		{0}),
		mlc::CommandType(L"print",		1,	{},		{}),
		mlc::CommandType(L"drawflush",	1,	{},		{0}),
		mlc::CommandType(L"printflush",	1,	{},		{0}),
		mlc::CommandType(L"getlink",		{2},	{0},	{1}),
		mlc::CommandType(L"control",		{6},	{}, {}),
		mlc::CommandType(L"radar",		7,	{6},	{0,1,2,3,4,5}),
		mlc::CommandType(L"sensor",		3,	{0},	{1,2}),
		mlc::CommandType(L"set",			{2},	{0}, {}),
		mlc::CommandType(L"op",			4,  {1},	{0}),
		mlc::CommandType(L"lookup",		3,	{1},	{0}),
		mlc::CommandType(L"packcolor",	5,	{0},	{}),
		mlc::CommandType(L"wait",		1,	{},		{}),
		mlc::CommandType(L"stop",		0,	{},		{}),
		mlc::CommandType(L"end",		0,	{},		{}),
		mlc::CommandType(L"jump",		4,	{},		{0,1}),
		mlc::CommandType(L"ubind",		1,	{},		{0}),
		mlc::CommandType(L"ucontrol",	6,	{},		{0}),
		mlc::CommandType(L"uradar",		7,	{6},	{0,1,2,3,4,5}),
		mlc::CommandType(L"ulocate",	8,	{4,5,7}, {0,1,2}),
	});

	bool find_command_type(const std::wstring_view name, mlc::CommandType& type) noexcept;

	mlc::Command make_command(const std::wstring_view name, const typename Command::args_type& args);
}
