#pragma once

#include "Commands.hpp"

namespace mlc
{
	class Operator;

	class OperatorType
	{
	public:
		using name_type = char;
		using converted_type = std::add_pointer_t<mlc::Command(const std::string&, const std::string&)>;

		OperatorType() noexcept;
		OperatorType(const name_type& name, converted_type converted = nullptr) noexcept;

		OperatorType(const Operator& op) noexcept;

		name_type name() const noexcept;
		bool operator<(const OperatorType& right) const noexcept;

	protected:
		name_type m_name;
		converted_type m_converted;
	};

	class Operator : public OperatorType
	{
		using Base = OperatorType;
	public:
		using name_type = typename Base::name_type;
		using argument_type = std::string;
		using converted_type = typename Base::converted_type;

		Operator() noexcept;
		Operator(const name_type& name, converted_type converted, const argument_type& first, const argument_type& second) noexcept;

		Operator(const argument_type& first, const argument_type& second, const OperatorType& type) noexcept;

		const argument_type& first() const noexcept;
		const argument_type& second() const noexcept;

		mlc::Command convert_to_command() const noexcept;

	protected:
		argument_type m_first_arg, m_second_arg;
	};

	const std::set<OperatorType> OPERATOR_LIST({
		OperatorType('=', [](auto first, auto second) { return mlc::Command("set", { first, second }, 0); }),
		//OperatorType('+', [](auto first, auto second) { return mlc::Command("op", {}, 0); }),
	});

	bool find_operator_type(const typename mlc::OperatorType::name_type opname, mlc::OperatorType& outoptype) noexcept;

}
