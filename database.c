#include "main.h"

extern char *main_database_err_msg;

int initialize_vault(sqlite3 *database) {
	char *sql = "BEGIN TRANSACTION;"
				"CREATE TABLE IF NOT EXISTS vault(id INTEGER PRIMARY KEY,"
												 "chest_name TEXT,"
												 "chest_balance INTEGER,"
												 "family_id INTEGER,"
												 "UNIQUE(chest_name));"
				"INSERT OR IGNORE INTO vault VALUES (1, 'root_account', 0, 0);"
				"COMMIT;";
	int rc = sqlite3_exec(database, sql, 0, 0, &main_database_err_msg);
	return(rc);
}

int create_new_chest(sqlite3 *database, char *name, int balance, int family) {
	sqlite3_stmt *res;
	int rc;
	char *sql = "INSERT OR IGNORE INTO vault(chest_name, chest_balance, family_id)"
			    "VALUES (@nam, @bal, @fam);";
	
	if ((rc = sqlite3_prepare_v2(database, sql, -1, &res, 0)) == SQLITE_OK) {
		int nam_idx = sqlite3_bind_parameter_index(res, "@nam");
		int bal_idx = sqlite3_bind_parameter_index(res, "@bal");
		int fam_idx = sqlite3_bind_parameter_index(res, "@fam");
		
		sqlite3_bind_text(res, nam_idx, name, -1, SQLITE_STATIC);
		sqlite3_bind_int(res, bal_idx, balance);
		sqlite3_bind_int(res, fam_idx, family);
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(database));
	}

	if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(database));
	}
	
	sqlite3_finalize(res);
	
	return(0);
}
