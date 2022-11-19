#include "Parsing.hpp"
#include "Variables.hpp"
#include "Commands.hpp"
#include "Line.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>

int main(int argc, char* argv[])
{
	(void)mlc::COMMAND_LIST;

	std::string infilepath;
	std::string outfilepath;

	if (argc == 2) {
		infilepath = argv[1];
		outfilepath = "out.txt";
	} else if (argc == 3) {
		infilepath = argv[1];
		outfilepath = argv[2];
	} else {
		//std::cerr << "\nWrong number of arguments\n";
		//std::exit(1);
		infilepath = "t.txt";
		outfilepath = "out.txt";
	}

	std::clog << "Opening (" << infilepath << ")\n";
	std::fstream infile(infilepath, std::fstream::in);
	if (!infile.is_open()) {
		std::cerr << "Input file open error\n";
		std::exit(1);
	}

	std::fstream outfile(outfilepath, std::fstream::trunc | std::fstream::out);
	if (!outfile.is_open()) {
		std::cerr << "Output file open error\n";
		std::exit(1);
	}

	bool is_compilation_with_error = false;
	auto compilation_error = [&](const mlc::Line& line) 
	{
		std::cerr << line.line() << " " << line.get() << " -> " << "Compilation error\n";
		is_compilation_with_error = true;
		//std::exit(1);
	};

	mlc::VariablesPool varpool;

	std::clog << "Compilation...\n\n";

	// get line
	mlc::Line line;
	while (std::getline(infile, (++line).get()))
	{

		// skip a line if it is empty
		if (line.get().find_first_not_of(' ') == std::string::npos) continue;
		mlc::uncomment_line(line);

		// get string using a delimiter
		std::stringstream ss{line.get()};
		while (std::getline(ss, line.get(), ';'))
		{
			// skip a line if it is empty
			if (line.get().find_first_not_of(' ') == std::string::npos) continue;

			mlc::Command command;
			mlc::Operator op;

			if (mlc::extract_operator(line, op)) {
				command = op.convert_to_command();
			}
			else
			if (mlc::extract_command(line, command)) {
				
			}
			else {
				compilation_error(line);
				continue;
			}

			mlc::raw_mlog_command_type rawcommand = command.convert();
			// write mlog command to output file
			outfile << rawcommand << '\n';

			// print status
			std::cout << line.get() << "\t -> " << rawcommand << '\n';

			if (!mlc::is_command_variables_valid(varpool, command)) {
				std::cerr << "^^^ Using unknown variable ^^^\n";
				is_compilation_with_error = true;
			}

			if (mlc::is_creating_var(command)) {
				if (varpool.add(command.out_arg())) {
					//std::cout << "Adding var\n";
				} else {
					std::cerr << "^^^ Invalid variable ^^^\n";
					is_compilation_with_error = true;
					//compilation_error(line);
				}
			}
		}
	}

	// save files
	infile.close();

	if (is_compilation_with_error) {
		outfile << "\n# COMPILATION FAILED WITH AN ERROR";
	}

	outfile.close();
	
	std::cin.get();
	return 0;
}
