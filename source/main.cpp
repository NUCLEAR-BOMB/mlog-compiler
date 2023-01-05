#include "IOFile.hpp"

#include <utility>
#include <iostream>

void process_command(const mlc::Line& command_line)
{
	std::cout << command_line.str() << '\n';
}

void iterate_commands(mlc::Line file_line)
{
	mlc::Line command_line;
	while (file_line.str().empty())
	{
		command_line = file_line.pop_subline(';');

		process_command(command_line);
	}
}

void iterate_lines(mlc::InputFile& file)
{
	while (!file.is_eof())
	{
		iterate_commands(file.read_next_line());
	}
}

int main()
{
	const mlc::string_t infile_name = "in.txt";
	const mlc::string_t outfile_name = "out.txt";

	mlc::InputFile infile(infile_name);
	mlc::OutputFile outfile(outfile_name);

	iterate_lines(infile);

	

	return 0;
}
