#include "database.h"
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
				"INSERT OR IGNORE INTO vault VALUES (2, 'uncategorized', 0, 0);"
				"COMMIT;";
	int rc = sqlite3_exec(database, sql, 0, 0, &main_database_err_msg);
	return(rc);
}

int initialize_uncategorized(sqlite3 *database, int balance) {
	sqlite3_stmt *res;
	int rc;
	char *sql = "UPDATE vault SET chest_balance=@bal WHERE id=2;";

	if ((rc = sqlite3_prepare_v2(database, sql, -1, &res, 0)) == SQLITE_OK) {
		int bal_idx = sqlite3_bind_parameter_index(res, "@bal");
		sqlite3_bind_int(res, bal_idx, balance);
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	sqlite3_finalize(res);

	return(0);
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
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	sqlite3_finalize(res);

	if (decrement_uncategorized_balance(database, balance) != 0) {
		fprintf(stderr, "Failed to decrement uncategorized balance.\n");
		return(-1);
	}

	return(0);
}

int update_chest_balance(sqlite3 *database, int id, int balance) {
	sqlite3_stmt *res;
	int rc;
	char *sql = "UPDATE vault SET chest_balance=@bal WHERE id=@id;";

	if ((rc = sqlite3_prepare_v2(database, sql, -1, &res, 0)) == SQLITE_OK) {
		int bal_idx = sqlite3_bind_parameter_index(res, "@bal");
		int id_idx  = sqlite3_bind_parameter_index(res, "@id");

		sqlite3_bind_int(res, bal_idx, balance);
		sqlite3_bind_int(res, id_idx, id);
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	if ((rc = sqlite3_step(res)) != SQLITE_DONE) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	sqlite3_finalize(res);

	return(0);
}

int increment_chest_value(sqlite3 *database, int id, int increment_amount) {
	sqlite3_stmt *res;
	int rc;
	char *sql_chest = "SELECT chest_balance FROM vault WHERE id=@id;";

	if ((rc = sqlite3_prepare_v2(database, sql_chest, -1, &res, 0)) == SQLITE_OK) {
		int id_idx = sqlite3_bind_parameter_index(res, "@id");

		sqlite3_bind_int(res, id_idx, id);
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	while (sqlite3_step(res) == SQLITE_ROW) {
		int bal = sqlite3_column_int(res, 0);
		bal += increment_amount;
		update_chest_balance(database, id, bal);
	}

	sqlite3_finalize(res);

	if (decrement_uncategorized_balance(database, increment_amount) != 0) {
		fprintf(stderr, "Failed to decrement uncategorized balance.\n");
		return(-1);
	}

	return(0);
}

int decrement_uncategorized_balance(sqlite3 *database, int decrement_amount) {
	sqlite3_stmt *res;
	int rc;
	char *sql_uncat = "SELECT chest_balance FROM vault WHERE id=2;";

	if ((rc = sqlite3_prepare_v2(database, sql_uncat, -1, &res, 0)) != SQLITE_OK) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	while (1) {
		int s = sqlite3_step(res);
		if (s == SQLITE_ROW) {
			int bal = sqlite3_column_int(res, 0);
			bal -= decrement_amount;
			update_chest_balance(database, 2, bal);
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			return(-1);
		}
	}

	sqlite3_finalize(res);

	return(0);
}

int read_chest_balance(sqlite3 *database, int id) {
	sqlite3_stmt *res;
	int rc, chest_balance = 0;
	const char *string_chest_balance;
	char *sql = "SELECT chest_balance FROM vault WHERE id=@id;";

	if ((rc = sqlite3_prepare_v2(database, sql, -1, &res, 0)) == SQLITE_OK) {
		int id_idx = sqlite3_bind_parameter_index(res, "@id");
		sqlite3_bind_int(res, id_idx, id);
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		exit(1);
	}

	while (1) {
		int s = sqlite3_step(res);
		if (s == SQLITE_ROW) {
			string_chest_balance = (const char *)sqlite3_column_text(res, 0);
			chest_balance = atoll(string_chest_balance);
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			sqlite3_close(database);
			exit(1);
		}
	}

	sqlite3_finalize(res);

	return(chest_balance);
}

int get_chest_id(sqlite3 *database, char *name) {
	sqlite3_stmt *res;
	int rc;
	int id = -1;
	const char *string_id;
	char *sql = "SELECT id FROM vault WHERE chest_name=@chest_name;";

	if ((rc = sqlite3_prepare_v2(database, sql, -1, &res, 0)) == SQLITE_OK) {
		int nam_idx = sqlite3_bind_parameter_index(res, "@chest_name");

		sqlite3_bind_text(res, nam_idx, name, -1, SQLITE_STATIC);
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}

	while (1) {
		int s = sqlite3_step(res);
		if (s == SQLITE_ROW) {
			string_id = (const char *)sqlite3_column_text(res, 0);
			id = atoll(string_id);
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			return(-1);
		}
	}

	sqlite3_finalize(res);

	if (id < 0)
		fprintf(stderr, "Unable to find a chest with that name.\n");

	return(id);
}

int calculate_grand_total(sqlite3 *database) {
	sqlite3_stmt *res;
	int rc;
	int grand_total = 0;
	char *sql = "SELECT chest_balance FROM vault where id != 1;";

	if ((rc = sqlite3_prepare_v2(database, sql, -1, &res, 0)) != SQLITE_OK) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		exit(1);
	}

	while (1) {
		int s = sqlite3_step(res);
		if (s == SQLITE_ROW) {
			int row_balance = sqlite3_column_int(res, 0);
			printf("Row balance: %d\n", row_balance);
			grand_total += row_balance;
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			sqlite3_close(database);
			exit(1);
		}
	}

	sqlite3_finalize(res);

	update_chest_balance(database, 1, grand_total);

	return(grand_total);
}

int print_chest(sqlite3 *database, char *chest_name) {
	sqlite3_stmt *res;
	int rc, chest_id;

	// I guess I don't need to do this... actually I probably should so that
	// I can verify that there is a chest with this name...
	if ((chest_id = get_chest_id(database, chest_name)) == -1) {
		fprintf(stderr, "Failed to locate chest\n");
		return(-1);
	}

	char *sql = "SELECT chest_balance, family_id FROM vault WHERE id=@chest_id;";

	if ((rc = sqlite3_prepare_v2(database, sql, -1, &res, 0)) == SQLITE_OK) {
		int id_idx = sqlite3_bind_parameter_index(res, "@chest_id");
		sqlite3_bind_int(res, id_idx, chest_id);
	} else {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(database));
		return(-1);
	}
	
	while (1) {
		int s = sqlite3_step(res);
		if (s == SQLITE_ROW) {
			int row_balance = sqlite3_column_int(res, 0);
			int row_family = sqlite3_column_int(res, 1);
			printf("Chest_name: %s -- Row_balance: %d Row_family: %d", chest_name, row_balance, row_family);
		} else if (s == SQLITE_DONE) {
			break;
		} else {
			fprintf(stderr, "Failed to read row.\n");
			sqlite3_close(database);
			exit(1);
		}
	}
	
	sqlite3_finalize(res);

	return(0);
}

int print_cb(void *p, int argc, char **argv, char **column) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s\t", argv[i]);
	}
	printf("\n");
	return(0);
}

int print_table(sqlite3 *db) {
	char *sql = "SELECT * FROM vault;";

	if (sqlite3_exec(db, sql, print_cb, NULL, &main_database_err_msg)) {
		fprintf(stderr, "Failed to execute: %s\n", sqlite3_errmsg(db));
		return(-1);
	}
	return(0);
}
