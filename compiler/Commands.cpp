#include "Commands.hpp"

#include <iostream>
#include <stdexcept>

mlc::Command::Command(const CommandType& type) noexcept
    : m_args(), Base(type)
{}

mlc::Command::Command() noexcept
{}

mlc::Command::Command(const std::wstring& command, const args_type& args, out_arg_type out_arg_index) noexcept
    : m_args(args), Base(command, args.size(), out_arg_index) {}

mlc::Command::Command(const args_type& args, const CommandType& cmdtype) noexcept
    : m_args(args), Base(cmdtype)
{}

const mlc::Command::args_type& mlc::Command::args() const noexcept {
    return m_args;
}

mlc::CommandType mlc::Command::type() const noexcept {
    return CommandType(m_command, Base::arg_count());
}

mlc::raw_mlog_command_type mlc::Command::convert() const noexcept
{
    using namespace std::string_literals;

    raw_mlog_command_type out = m_command;
    for (const auto& s : m_args) {
        out += L" "s;
        out += s;
    }
    return out;
}

const mlc::Command::argument_type& mlc::Command::operator[](std::size_t index) const {
    try {
        return m_args.at(index);
    } catch (...) {
        throw;
    }
}

mlc::CommandType::CommandType() noexcept
    : m_command(), m_arguments_count(123456789), m_out_arg(1234567)
{}

mlc::CommandType::CommandType(
    const std::wstring_view command,
    arguments_count_type arguments_count, 
    out_arg_type out_arg_, 
    const ignore_args_type& ignore_args
) noexcept
    : m_command(command)
    , m_arguments_count(arguments_count)
    , m_out_arg(out_arg_)
    , m_ignore_args(ignore_args)
    , m_has_out_args(!out_arg_.empty())
{}

mlc::CommandType::CommandType(const Command& cmd) noexcept
    : m_command(cmd.name()), m_arguments_count(cmd.arg_count())
{}

const std::wstring& mlc::CommandType::name() const& noexcept {
    return m_command;
}

mlc::CommandType::arguments_count_type mlc::CommandType::arg_count() const noexcept {
    return m_arguments_count;
}

mlc::CommandType::out_arg_type mlc::CommandType::out_arg_index() const noexcept {
    return m_out_arg;
}

const mlc::CommandType::ignore_args_type& mlc::CommandType::ignore_args() const noexcept {
    return m_ignore_args;
}

bool mlc::CommandType::has_out_args() const noexcept {
    return m_has_out_args;
}

bool mlc::CommandType::operator==(const CommandType& right) const noexcept {
    return name() == right.name() && m_arguments_count == right.m_arguments_count;
}

bool mlc::CommandType::operator<(const CommandType& right) const noexcept {
    return m_command < right.m_command;
}

bool mlc::is_creating_var(const mlc::CommandType& cmdtype) noexcept {
    return cmdtype.has_out_args();
}

std::wstring mlc::create_temp_variable_name(mlc::Line::line_counter_type l) noexcept {
    using namespace std::string_literals;
    return L"__TEMP_"s + std::to_wstring(l) + L"__"s;
}

bool mlc::find_command_type(const std::wstring_view name, mlc::CommandType& type) noexcept
{
    // check if COMMAND_LIST contains command.type() 
    //auto t = mlc::COMMAND_LIST.find(mlc::CommandType(cmdname));
    auto t = mlc::COMMAND_LIST.find(mlc::CommandType(name));

    if (t != mlc::COMMAND_LIST.cend()) {
        type = *t;
        return true;
    }
    return false;
}

mlc::Command mlc::make_command(const std::wstring_view name, const typename Command::args_type& args)
{
    mlc::CommandType cmd;
    if (!mlc::find_command_type(name, cmd)) throw std::logic_error("Unknown name");
    return mlc::Command(args, cmd);
}
