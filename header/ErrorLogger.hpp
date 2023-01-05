#pragma once

#include "TypeAliases.hpp"

#include <iostream>
#include <cstdlib>

namespace mlc
{

[[noreturn]] inline
void msglog(mlc::string_const_ref_t errmsg) noexcept 
{
	std::cerr << "Critical error: " << errmsg << '\n';

	std::exit(1);
}

}
