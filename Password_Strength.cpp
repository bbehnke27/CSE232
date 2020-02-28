
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

using std::ifstream;
using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::istringstream;
using std::cin;
using std::cout;
using std::pair;
using std::endl;

vector<string> open_file() {
	vector<string> password;
	string temp;
	ifstream in_file("common_passwords.txt");
	while (getline(in_file, temp)) {
		istringstream iss(temp);
		password.push_back(temp);	
	}
	return password;
}
pair<string, int> find_similar(vector<string> passwords, string user_password) {
	string similar_passwords;
	string string_big;
	string string_small;
	int difference = 0;
	int smallest_difference = 999;

	std::sort(passwords.begin(), passwords.end());

	for (auto element : passwords) {
		if (static_cast<int>(user_password.length()) > static_cast<int>(element.length())) {
			string_big = user_password;
			string_small = element;
			difference += static_cast<int>(string_big.length()) - static_cast<int>(string_small.length());
		}
		else if (static_cast<int>(user_password.length()) < static_cast<int>(element.length())) {
			string_big = element;
			string_small = user_password;
			difference += static_cast<int>(string_big.length()) - static_cast<int>(string_small.length());
		}
		else if (static_cast<int>(user_password.length()) == static_cast<int>(element.length())) {
			string_big = user_password;
			string_small = element;
		}
		for (int i = 0; i < static_cast<int>(string_small.length()); i++) {
			if (string_big[i] != string_small[i]) {
				difference += 1;
			}
		}
		if (difference == smallest_difference) {
			similar_passwords.append(element);
			similar_passwords.append(", ");
		}
		if (difference < smallest_difference) {
			similar_passwords = "";
			smallest_difference = difference;
			similar_passwords.append(element);
			similar_passwords.append(", ");
		}
		difference = 0;
	}
	return make_pair(similar_passwords,smallest_difference);
}
int main()
{
	string user_password;
	vector<string> password = open_file();
	cout << "Give me a password:" << endl;
	cin >> user_password;
	pair<string,int> similar_passwords = find_similar(password, user_password);
	cout << "You provided a password of " << user_password << endl;
	cout << "The most similar passwords to " << user_password << " are:" << endl;
	cout << similar_passwords.first << endl;
	cout << "All of which are " << similar_passwords.second << " character(s) different." << endl;
}
