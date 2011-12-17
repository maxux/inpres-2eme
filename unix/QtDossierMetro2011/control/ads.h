#ifndef __ADS_H
	#define __ADS_H
	
	#include <sqlite3.h>
	
	#define ADS_SQL_FILE		"ads.sqlite3"
	#define ADS_MAX_LENGTH		1024
	#define ADMIN_MESSAGE_DELAY	4
	
	void ads_show();
	
	sqlite3 * ads_init();
	int ads_close();
	
	sqlite3_stmt * ads_select_query(sqlite3 *db, char *sql);
	int ads_simple_query(sqlite3 *db, char *sql);
	
	int ads_load();
	int ads_content(int id, int *timeout, char *text, size_t textlen);
	
#endif
