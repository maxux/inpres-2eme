#include "ExceptionBase.hxx"
#include <string.h>

ExceptionBase::ExceptionBase() {
	_message = NULL;
}

ExceptionBase::ExceptionBase(const char *message) {
	_message = NULL;
	setMessage(message);
}

ExceptionBase::ExceptionBase(const ExceptionBase &original) {
	_message = NULL;
	
	if(original.getMessage() != NULL)
		setMessage(original.getMessage());
}

ExceptionBase::~ExceptionBase() {
	if(_message != NULL)
		delete _message;
}

void ExceptionBase::setMessage(const char *message) {
	if(_message != NULL)
		delete _message;
	
	_message = new char[strlen(message) + 1];
	strcpy(_message, message);
}

const char * ExceptionBase::getMessage() const {
	return _message;
}
