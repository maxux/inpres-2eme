#include <iostream>
#include <ctime>
#include <cstring>
#include "FichierLog.hxx"

using namespace std;

/* File Handling */
FichierLog::FichierLog(const char *filename) {
	_out.open(filename);
}

FichierLog::~FichierLog() {
	_out.close();
}

/* Input/Output */
FichierLog & operator << (FichierLog &stream, const char *data) {
	time_t rawtime;
	struct tm * timeinfo;
	int dlen;
	char *output, *date;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	date = asctime(timeinfo);
	
	dlen = strlen(date);
	date[dlen-- - 1] = '\0';
		
	/* Format: <date> <message> */
	output = new char[dlen + strlen(data) + 2];
	
	sprintf(output, "%s %s", date, data);
	stream.Write(output);
	
	delete output;
	
	return stream;
}

void FichierLog::Write(char *data) {
	_out << data << endl;
}
