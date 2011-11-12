#include "AuthorisedPerson.hxx"
#include <string.h>
using namespace std;

AuthorisedPerson::AuthorisedPerson() : Person() {
	*_login = '\0';
}

AuthorisedPerson::AuthorisedPerson(const char *name, const char *surname, const char *born, const char *nation, const char *login) : Person(name, surname, born, nation) {
	setLogin(login);
}

AuthorisedPerson::AuthorisedPerson(AuthorisedPerson &original) : Person(original) {
	strcpy(_login, original.getLogin());
}

void AuthorisedPerson::setLogin(const char *login) {
	strcpy(_login, login);
}

const char * AuthorisedPerson::getLogin() {
	return _login;
}
