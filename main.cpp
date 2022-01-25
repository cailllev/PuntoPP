#include <algorithm>
#include <cstring>
#include <iostream>
#include "test.h"
#include "heuristics.h"
#include "board.h"
#include "card.h"

// https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
char * getCmdOption(char ** begin, char ** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}


void test() {
	if (!tests()) {
		std::cout << "Tests failed!\n";
	} else {
		std::cout << "Tests successful!\n";
	}	
}

const char * help() {
	const char *help_text =
		"Punto the Game! but in C++\n"
		"Usage: \n"
		"  -h    print this\n"
		"  -p    do a round of Punto with players, i.e. '-p Alice,Bob,Charlie,Dave'"
		"  -t    do tests\n";
	return help_text;
}


int main(int argc, char * argv[]) {
	if(cmdOptionExists(argv, argv+argc, "-h")) {
        std::cout << help();
        return 0;
    }

	if(cmdOptionExists(argv, argv+argc, "-t")) {
        test();
    }

	if(cmdOptionExists(argv, argv+argc, "-p")) {
		std::list<std::string> players_parsed = std::list<std::string>();
	    char *players = getCmdOption(argv, argv + argc, "-p");
	    if (!players) {
	    	std::cout << help();
	    	return 0;
	    }
	    char *player = strtok(players, ",");
	    while(player) {
	    	players_parsed.push_back(player);
	        player = strtok(NULL, ",");
	    }
	    Game *g = new Game(players_parsed);
	    g->play_round();
    }

	return 0;
}

