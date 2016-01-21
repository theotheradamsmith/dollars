#include "main.h"
#include "database.h"
#include "string_util.h"

char *main_database_err_msg = NULL;

int main(void) {
	FILE *fp;
	if ((fp = fopen(ROOT_DB, "ab+"))) {
		fclose(fp);
	} else {
		fprintf(stderr, "FATAL ERROR\n");
		exit(1);
	}

	sqlite3 *db;
	int rc;

	if ((rc = sqlite3_open(ROOT_DB, &db)) != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	if ((rc = initialize_vault(db)) != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", main_database_err_msg);
		sqlite3_free(main_database_err_msg);
		sqlite3_close(db);
		exit(1);
	}

	create_new_chest(db, "test", 0, KITCHEN);

	sqlite3_close(db);

	return(0);
}

