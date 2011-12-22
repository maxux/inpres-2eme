#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sqlite3.h>

#include "ads.h"
#include "control.h"
#include "stack_client.h"
#include "debug.h"
#include "ipc_messages.h"

extern global_t sys;

void ads_show() {
	int i, timeout;
	char text[SHARED_MEMORY_SIZE];
	
	debug("ADS: (SIG) Sending advertissment...\n");
	
	if(sys.admin_msg) {
		/* Reset Flag */
		sys.admin_msg = 0;
		
		/* Sending Admin Message to clients */
		stack_sending_signal(*(sys.clients_head), SIGUSR2, 0);
		
		/* Waiting for resume */
		usleep(ADMIN_MESSAGE_DELAY * 1000000);
	}
	
	while(1) {
		if(!sys.ads_count)
			sys.ads_count = 1;
			
		for(i = 0; i < sys.ads_count; i++) {
			/* Waiting until clients connection */
			/* while(!sys.clients_head)
				usleep(1000000); */
			
			if(!ads_content(sys.ads_index[i], &timeout, text, SHARED_MEMORY_SIZE)) {
				debugc("ADS: (ERR) Cannot read SQL message %d\n", sys.ads_index[i]);
				usleep(1000000);
				continue;
			}
			
			/* Copy ads to shared memory */
			strcpy(sys.shm, text);
			
			debug("ADS: (SND) Sending advertisement %d (%s). Timeout: %d\n", sys.ads_index[i], text, timeout);
			
			/* Sending Ads to clients */
			stack_sending_signal(*(sys.clients_head), SIGUSR1, 0);
			
			/* Waiting next ads */
			usleep(timeout * 1000000);
		}
	}
}

sqlite3_stmt * ads_select_query(sqlite3 *db, char *sql) {
	sqlite3_stmt *stmt;
	
	/* Debug SQL */
	debug("SQL: (QRY) <%s>\n", sql);
	
	/* Query */
	if(sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
		debugc("ADS: (ERR) Query <%s> failed: %s\n", sql, sqlite3_errmsg(db));
		return NULL;
	}
	
	return stmt;
}

int ads_simple_query(sqlite3 *db, char *sql) {
	/* Debug SQL */
	debug("SQL: (QRY) <%s>\n", sql);
	
	/* Query */
	if(sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		debugc("ADS: (ERR) Query <%s> failed: %s\n", sql, sqlite3_errmsg(db));
		return 0;
	}
	
	return 1;
}

sqlite3 * ads_init() {
	sqlite3 *db;
	
	debug("ADS: (SQL) Opening %s\n", ADS_SQL_FILE);
	
	if(sqlite3_open(ADS_SQL_FILE, &db) != SQLITE_OK) {
		debugc("ADS: (ERR) Cannot open sqlite databse: %s\n", sqlite3_errmsg(db));
		return NULL;
	}
	
	return db;
}

int ads_load() {
	sqlite3_stmt *stmt;
	int i, row, sql_timeout, sql_id;
	const unsigned char *sql_text;
	
	/* Reading table content */
	if((stmt = ads_select_query(sys.ads_db, "SELECT id, text, timeout FROM ads")) == NULL)
		return 0;
	
	/* Counting... */
	sys.ads_count = 0;
	while((row = sqlite3_step(stmt)) != SQLITE_DONE) {
		if(row == SQLITE_ROW)
			sys.ads_count++;
	}
	
	debug("ADS: (SQL) %d rows returned\n", sys.ads_count);
	
	/* Reset statement */
	sqlite3_reset(stmt);
	
	/* Clearing old index */
	if(sys.ads_index != NULL)
		free(sys.ads_index);
	
	/* Allocating index */
	sys.ads_index = (int*) malloc(sizeof(int) * sys.ads_count);
	if(!sys.ads_index) {
		debugc("ADS: (ERR) Allocation failed.\n");
		return 0;
	}
	
	/* Loading table */
	i = 0;
	while((row = sqlite3_step(stmt)) != SQLITE_DONE) {
		switch(row) {
			case SQLITE_ROW:
				sql_id      = sqlite3_column_int(stmt, 0);
				sql_text    = sqlite3_column_text(stmt, 1);
				sql_timeout = sqlite3_column_int(stmt, 2);
				
				debug("ADS: (DBG) -> (ID: %d | Timeout: %d) <%s>\n", sql_id, sql_timeout, sql_text);
				sys.ads_index[i] = sql_id;
				
				i++;
			break;
			
			default:
				debugc("ADS: (SQL) WTF ? %d\n", row);
		}
	}
	
	/* Clearing */
	sqlite3_finalize(stmt);
	
	debugn("ADS: Advertisements loaded: %d rows read\n", i);
	
	return 1;
}

int ads_content(int id, int *timeout, char *text, size_t textlen) {
	sqlite3_stmt *stmt;
	int row;
	const unsigned char *sql_text;
	char sql[256];
	
	/* Building SQL Query */
	sprintf(sql, "SELECT text, timeout FROM ads WHERE id = %d", id);
	
	/* Reading table content */
	if((stmt = ads_select_query(sys.ads_db, sql)) == NULL)
		return 0;
	
	/* Loading table */
	if((row = sqlite3_step(stmt)) != SQLITE_DONE && row == SQLITE_ROW) {
		sql_text = sqlite3_column_text(stmt, 0);
		
		strncpy(text, (const char *) sql_text, textlen);
		text[textlen - 1] = '\0';
		
		*timeout = sqlite3_column_int(stmt, 1);
	} else {
		*timeout = 5;
		strcpy(text, "Aucune informations disponible");
		
		debugc("ADS: (ERR) Malformed SQL reply\n");
	}
	
	sqlite3_finalize(stmt);
	
	return 1;
}

int ads_close() {
	debug("ADS: (SQL) Closing sqlite database...\n");
	
	/* Clearing Index */
	if(sys.ads_index != NULL)
		free(sys.ads_index);
	
	/* Closing Database */
	if(sqlite3_close(sys.ads_db) != SQLITE_OK) {
		debugc("ADS: (ERR) Cannot close sqlite databse: %s\n", sqlite3_errmsg(sys.ads_db));
		return 0;
		
	} else return 1;
}
