#include <stdio.h>
#include <sqlite3.h>

#include "debug.h"
#include "ads.h"
#include "control.h"

sqlite3 * init_sql() {
	sqlite3 *db;
	
	debug("SQL: Opening %s\n", ADS_SQL_FILE);
	
	if(sqlite3_open(ADS_SQL_FILE, &db) != SQLITE_OK) {
		debugc("SQL: Cannot open sqlite databse: %s\n", sqlite3_errmsg(db));
		return NULL;
	}
	
	return db;
}

void open_ads(global_t *sys) {
	// sys->sqlite;
}
