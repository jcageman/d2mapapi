#include <iostream>
#include <string>
#include <regex>

std::string extractArg(std::string arg) {
	std::regex rgx("--\\w+=(.+)");
	std::smatch match;
	if (std::regex_search(arg, match, rgx)) {
		return match[1];
	}
	else {
		std::cerr << "Failed to extract value in argument: " + arg;
		return "";
	}
}

void printHelp() {
	std::cerr << "Usage: program.exe {diablo2dir} [optionalArguments]\n";
	std::cerr << "\t-i or --ip\n\t\tspecify an IP address to use instead of localhost.\n\t\tex: -i 192.168.0.1 OR --ip=192.168.0.1\n";
	std::cerr << "\t-p or --port\n\t\tspecify an port address to use instead of the default 8080.\n\t\tex: -p 8080 OR --port=8080\n";
	std::cerr << "\t-h or --help\n\t\tprints this usage information\n";
	std::cerr << "Example: program.exe \"C:\\Diablo II1.13c\"\nor\nExample: program.exe \"C:\\Diablo II1.13c\" -i 0.0.0.0 -p 8888";
}