#ifndef __ADS_H
	#define __ADS_H
	
	#include <sqlite3.h>
	
	#define ADS_SQL_FILE	"ads.sqlite3"
	
	void ads_show();
	
	sqlite3 * ads_init();
	int ads_close();
	
	int ads_load();
	int ads_content(int id, int *timeout, char *text, size_t textlen);
	
#endif
