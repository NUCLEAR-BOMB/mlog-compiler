#include "Operators.hpp"

mlc::Operator::Operator() noexcept
{}

mlc::Operator::Operator(const name_type& name, converted_type converted, const argument_type& first, const argument_type& second) noexcept
	: m_first_arg(first), m_second_arg(second), Base(name, converted)
{}

mlc::Operator::Operator(const argument_type & first, const argument_type & second, const OperatorType& type) noexcept
	: m_first_arg(first), m_second_arg(second), Base(type)
{}

const mlc::Operator::argument_type& mlc::Operator::first() const noexcept {
	return m_first_arg;
}

const mlc::Operator::argument_type& mlc::Operator::second() const noexcept {
	return m_second_arg;
}

mlc::Command mlc::Operator::convert_to_command() const noexcept {
	return Base::m_converted(m_first_arg, m_second_arg);
}

mlc::OperatorType::OperatorType() noexcept
	: m_name(-123), m_converted(nullptr)
{}

mlc::OperatorType::OperatorType(const name_type& name, converted_type converted) noexcept
	: m_name(name), m_converted(converted)
{}

mlc::OperatorType::OperatorType(const Operator & op) noexcept
	: m_name(op.name())
{}

mlc::OperatorType::name_type mlc::OperatorType::name() const noexcept {
	return m_name;
}

bool mlc::OperatorType::operator<(const OperatorType& right) const noexcept {
	return m_name < right.m_name;
}

bool mlc::find_operator_type(const typename mlc::OperatorType::name_type opname, mlc::OperatorType& outoptype) noexcept
{
	auto t = mlc::OPERATOR_LIST.find(mlc::OperatorType(opname));

	if (t != mlc::OPERATOR_LIST.cend()) {
		outoptype = *t;
		return true;
	}
	return false;
}
