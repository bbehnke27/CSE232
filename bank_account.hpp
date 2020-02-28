#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
#pragma once

double deposit(double& bank, double& amount) {
	// helper function to deposit money into an account

	bank += amount;
	return bank;
}

bool withdraw(double& bank, double amount) {
	// helper function to tell if it is possible to withdraw from an account
	// returns true if the account will not become overdrawn by removing "amount" from it.

	if (amount < bank) {
		bank -= amount;
		return true;
	}
	else {
		return false;
	}
}

void overdraft(double& bank, double amount) {
	// overdraft is calculated when a bank account goes below 0.
	// a flat fee of 35 dollars is added to the account if it becomes over drawn.
	bank -= amount;
	bank -= 35;
}

double interest_for_month(double& bank, double& apr) {
	// helper function that calculates the interest for one month. This function
	// "helps" the total interest calculation and is used in conjunction with the number of months
	// function to calculate a cumulative interest value over time.

	double apr_percent = apr / 12;
	double interest = bank * apr_percent;
	double sub = interest - floor(interest);
	double out = (interest - sub) / 100;
	return out;
}

void string_date_to_int_ptrs(const std::string& date, int* year, int* month, int* day) {
	// converts a string containing a date to pointers to a year, month, and day.

	*year = std::stoi(date.substr(0, 4));
	*month = std::stoi(date.substr(5, 2));
	*day = std::stoi(date.substr(8, 2));
}

void string_date_to_ints(const std::string& date, int& year, int& month, int& day) {
	// converts a string with a date to individual integer values corresponding to the day,
	// month, and year. does not return, but instead alters a global variable assigned to each value

	year = std::stoi(date.substr(0, 4));
	month = std::stoi(date.substr(5, 2));
	day = std::stoi(date.substr(8, 2));
}

int number_of_first_of_months(const std::string date_early, const std::string date_late) {
	// helper function to figure out the number of months between two dates. returns a double with
	// the number of months. The number of months is used to calculate the interest accrued over 
	// date_early to date_late.

	int total = 0;

	int year_early, month_early, day_early;
	string_date_to_ints(date_early, year_early, month_early, day_early);

	int year_late, month_late, day_late;
	string_date_to_ints(date_late, year_late, month_late, day_late);

	int month_difference = month_late - month_early;
	int year_difference = year_late - year_early;

	total += year_difference * 12;
	total += month_difference;

	return total;
}

double interest_earned(double bank, double apr, std::string date_early, std::string date_late) {
	// helper function that calculates interest over the span of date_early to date_late with a parameter
	// apr. Returns a double value with the total interest accrued over the time period.

	double total_interest = 0;
	double temp = bank;
	if (temp < 0) {
		return 0;
	}
	for (int i = 0; i < static_cast<double>(number_of_first_of_months(date_early, date_late)); i++) {
		total_interest += interest_for_month(temp, apr);
		temp += interest_for_month(temp, apr);
	}
	return total_interest;
}

std::string process_command(std::string line, std::string& previous_date, double& bank, double apr) {
	// function that processes a single line string which contains a date, a direction (withdraw or deposit)
	// and an amount of money. also takes in a current bank balance, an apr percentage, and a previously used
	// date. This function processes whether or not interest needs to be calculated, whether the users account is
	// overdrawn, and returns a string containing what exactly happened while the function is executed.

	std::string out;
	std::ostringstream oss;

	int previous_year, previous_month, previous_day;
	std::string end_date = line.substr(0, 10);
	if (previous_date == "") { // in case a previous date wasn't provided, sets the current date as previous date.
		previous_date = end_date;
	}
	string_date_to_ints(previous_date, previous_year, previous_month, previous_day);

	std::string w_or_d = line.substr(11, 1);

	int number_of_months = number_of_first_of_months(previous_date, end_date);

	std::string string_amount = (line.substr(line.find("$") + 1));
	double amount = std::stod(line.substr(line.find("$") + 1));

	if (w_or_d == "D") { // if the string asks the program to deposit
		out.append("On ");
		out.append(end_date);
		out.append(": Instructed to perform \"Deposit $");
		out.append(string_amount);
		out.append("\"");

		if (number_of_months > 0) { // checks if interest will be calculated
			out.append("\nSince ");
			out.append(previous_date);
			out.append(", interest has accrued ");
			out.append(std::to_string(number_of_months));
			out.append(" times.\n$");
			double interest = interest_earned(bank, apr, previous_date, end_date);
			bank += interest;
			oss.clear();
			oss << std::setprecision(2) << std::fixed << interest;
			std::string interest_string = oss.str();
			out.append(interest_string);
			out.append(" interest has been earned.");
			out.append("\nBalance: ");
			bank += amount;
			std::ostringstream oss2;
			oss2 << std::setprecision(2) << std::fixed << bank;
			std::string bank_string = oss2.str();
			out.append(bank_string);
			out.append("\n");
		}
		if (number_of_months == 0) {
			out.append("\nBalance: ");
			bank += amount;
			std::ostringstream oss3;
			oss3 << std::setprecision(2) << std::fixed << bank;
			std::string bank_string = oss3.str();
			out.append(bank_string);
			out.append("\n");
		}
	}

	else if (w_or_d == "W") { // If the string asks the program to withdraw
		out.append("On ");
		out.append(end_date);
		out.append(": Instructed to perform \"Withdraw $");
		out.append(string_amount);
		out.append("\"");

		if (number_of_months > 0) {  // checks if interest will need to be calculated
			out.append("\nSince ");
			out.append(previous_date);
			out.append(", interest has accrued ");
			out.append(std::to_string(number_of_months));
			out.append(" times.\n$");
			double interest = interest_earned(bank, apr, previous_date, end_date);
			bank += interest;
			std::ostringstream oss4;
			oss4 << std::setprecision(2) << std::fixed << interest;
			std::string interest_string = oss4.str();
			out.append(interest_string);
			out.append(" interest has been earned.");
			previous_date = end_date;
		}
		if (bank - amount > 0) {
			bank -= amount;
			out.append("\nBalance: ");
			std::ostringstream oss5;
			oss5 << std::setprecision(2) << std::fixed << bank;
			std::string bank_string = oss5.str();
			out.append(bank_string);
			out.append("\n");
		}
		else if (bank - amount < 0) {
			overdraft(bank, amount);
			out.append("\nOverdraft!");
			out.append("\nBalance: ");
			std::ostringstream oss6;
			oss6 << std::setprecision(2) << std::fixed << bank;
			std::string bank_string = oss6.str();
			out.append(bank_string);
			out.append("\n");
		}
	}
	previous_date = end_date;
	return out;
}

std::string process_commands(std::string in, double apr) {
	// process commands runs process command function multiple times, in the case that
	// multi line strings are entered as the first parameter.
	// returns a multi line string containing interest accumulated, as well as total balance

	double bank = 0;
	std::string out;
	std::string previous_date;
	std::string line;
	std::istringstream ss(in);
	while (getline(ss, line)) {
		out.append(process_command(line, previous_date, bank, apr));
	}
	return out;
}