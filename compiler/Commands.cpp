#include "Commands.hpp"

#include <iostream>

mlc::Command::Command() noexcept
{}

mlc::Command::Command(const std::string& command, const args_type& args, out_arg_type out_arg) noexcept
    : m_args(args), Base(command, args.size(), out_arg) {}

const mlc::Command::args_type& mlc::Command::args() const noexcept {
    return m_args;
}

mlc::CommandType mlc::Command::type() const noexcept {
    return CommandType(m_command, Base::arg_count(), 0);
}

mlc::raw_mlog_command_type mlc::Command::convert() const noexcept
{
    using namespace std::string_literals;
    raw_mlog_command_type out = m_command;
    for (const auto& s : m_args) {
        out += " "s;
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
    : m_command(), m_arguments_count(123456789), m_out_arg(-1234567)
{}

mlc::CommandType::CommandType(const std::string_view command, arguments_count_type arguments_count, out_arg_type out_arg_) noexcept
    : m_command(command), m_arguments_count(arguments_count), m_out_arg(out_arg_) {}

mlc::CommandType::CommandType(const Command& cmd) noexcept
    : m_command(cmd.name()), m_arguments_count(cmd.arg_count())
{}

const std::string& mlc::CommandType::name() const& noexcept {
    return m_command;
}

mlc::CommandType::arguments_count_type mlc::CommandType::arg_count() const noexcept {
    return m_arguments_count;
}

mlc::CommandType::out_arg_type mlc::CommandType::out_arg() const noexcept {
    return m_out_arg;
}

bool mlc::CommandType::operator==(const CommandType& right) const noexcept {
    return name() == right.name() && m_arguments_count == right.m_arguments_count;
}

bool mlc::CommandType::operator<(const CommandType& right) const noexcept {
    return m_command < right.m_command;
}
