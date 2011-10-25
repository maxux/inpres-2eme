#include "Date.hxx"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Date::Date() {
	_day   = 1;
	_month = 1;
	_year  = 1970;
	
	_to_char = new char[11];
}

Date::Date(Date &original) {
	_day   = original.getDay();
	_month = original.getMonth();
	_year  = original.getYear();
	
	_to_char = new char[11];
}

Date::Date(char *date) {
	_to_char = new char[11];
	setDate(date);
}

Date::~Date() {
	delete _to_char;
}

/* Setters */
void Date::setDate(const char *date) {
	_day   = atoi(date);
	_month = atoi(date + 3);
	_year  = atoi(date + 6);
}

/* Getters */
const char * Date::getDate() const {
	sprintf(_to_char, "%02d/%02d/%04d", _day, _month, _year);
	return _to_char;
}

unsigned short Date::getDay() {
	return _day;
}

unsigned short Date::getMonth() {
	return _month;
}

unsigned short Date::getYear() {
	return _year;
}
