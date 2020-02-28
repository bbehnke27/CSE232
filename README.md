CSE 232 Run Cellular Automata Project

#include <iostream>
#include <cmath>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;

int get_line_sum(const string string_cellular_automata) {
	//  This function takes in a string of CA, and returns the sum of every
	//  character in the line

	int total = 0;
	for (int i = 0; i < static_cast<int>(string_cellular_automata.length()); i++) {
		total += string_cellular_automata[i] - 48;
	}
	return total;  //  returns the sum of the line
}

int get_next_state(string string_neighborhood, const string string_rules) {
	//  This function takes in a neighborhood (three digit string), and a
	//  constant raw string literal containing rules. The function extracts
	//  the value associated with each neighborhood within the rules, and returns
	//  the value if the three digit string_neighborhood taken in matches one
	//  of the three digit strings found in the rule set.

	for (int i = 0; i < static_cast<int>(string_rules.length()); i += 9) {
		string number = string_rules.substr(i, 3);
		char out_number = string_rules[i + 7];
		if (number == string_neighborhood) {
			return out_number;  //  returns the number associated with the neighborhood tested
		}
	}
	char zero = '0';
	return zero;  //  returns the char '0' as a default
}

void update_line(string &string_line, const string &string_rules) {
	//  update_line function takes in a line of digits as a string, and a
	//  raw literal string of rules as seen in the previous function. The string_line
	//  is altered based on the output from running three neighboring digits in the 
	//  get_next_state function, and replacing the middle character with the output from
	//  the get_next_state function. The string_line is considered a loop, and the first
	//  character (index 0) neighbors the final character of the string.
	//  The function returns nothing, but instead alters string_line.

	string string_copy = string_line;  //  a copy of the string is made to make changes to
	int string_length = string_line.length();
	for (int i = 0; i < string_length - 1; i++) {
		string index = string_line.substr(i, 3);  //  iterates through the string, indexing 3 characters at a time
		char next_state = get_next_state(index, string_rules);  //  runs the 3 characters through get_next_state
		string_copy[i + 1] = next_state; 
	}
	string string_loop{string_line[string_length - 2], string_line[string_length - 1], string_line[0]};
	char next_state = get_next_state(string_loop, string_rules);  //  addresses the case where the final two
	string_copy[string_length-1] = next_state;                    //  characters neighbor the first.

	string string_loop_two{string_line[string_length - 1], string_line[0], string_line[1]};
	char next_state_two = get_next_state(string_loop_two, string_rules);
	string_copy[0] = next_state_two;  //  addresses the case where the final character neighbors the first two.
	
	string_line = string_copy;  //  does not return, but instead alters one of the parameters
}

string run_cellular_automata(string string_rules, const int iterations, string string_start) {
	//  This function takes in a raw literal string of rules as seen previously, an int representing the
	//  number of iterations, and the initial string to be altered by update_line.
	//  The function alters the take string (iteration) number of times and returns
	//  a multi line string containing the altered string_start and its sum (using the 
	//  get_line_sum function.

	string string_final;
	string start = string_start + " sum = ";
	string_final.append(start);
	string_final += std::to_string(get_line_sum(string_start));  //  converts the type int to char
	string_final.append("\n");
	for (int i = 1; i < iterations; i++) {  //  for loop updates the line (iterations) amount of times.
		update_line(string_start, string_rules);
		string_final.append(string_start);  //  appends the updated string to a multi-line string.
		string_final.append(" sum = ");  
		if (string_start == "0000000000") {
			string_final += '0';
			string_final.append("\n");
			break;
		}
		string_final += std::to_string(get_line_sum(string_start));
		string_final.append("\n");
	}
	return string_final;  //  returns a multi line string containing changed lines and their sum
}

int main() {
	//  The main function accepts input from cin, where the user inputs the multi-line raw
	//  string literal containing the rules, an integer representing the number of iterations,
	//  and the initial line to be altered. It makes a call to the run_cellular_automata function
	//  using the three inputs from cin, and outputs the altered string (number_of_lines) amount of
	//  times, as well as the sum of the given line (using get_line_sum)

	string string_rules;
	string string_temporary;
	while (getline(cin, string_temporary)) {
		if (string_temporary == ".") {
			break;
		}
		string_rules.append(string_temporary);
		string_rules+='\n';
	}
	int number_of_lines;
	cin >> number_of_lines;
	string starting_line;
	cin >> starting_line;
	cout << run_cellular_automata(string_rules, number_of_lines, starting_line) << endl;  // cout multi-line string
}
