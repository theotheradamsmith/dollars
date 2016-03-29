#include "main.h"
#include "database.h"
#include "string_util.h"

char *main_database_err_msg = NULL;

int main(void) {

	/* Check to see if the ROOT_DB file exists; if not, create it. Exit on fatal error. */
	FILE *fp;

	if ((fp = fopen(ROOT_DB, "ab+"))) {
		fclose(fp);
	} else {
		fprintf(stderr, "FATAL ERROR: Unable to open/create ROOT_DB!\n");
		exit(1);
	}

	/* Attempt to connect to the ROOT_DB and ensure that our primary vault table exists */
	sqlite3 *db;
	int rc;

	if ((rc = sqlite3_open(ROOT_DB, &db)) != SQLITE_OK) {
		fprintf(stderr, "FATAL ERROR: Cannot open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}

	putchar('\n');
	printf("Welcome to your Estate. Initializing...\n");

	if ((rc = initialize_vault(db)) != SQLITE_OK) {
		fprintf(stderr, "FATAL ERROR: SQL error: %s\n", main_database_err_msg);
		sqlite3_free(main_database_err_msg);
		sqlite3_close(db);
		exit(1);
	}

	/* Now that we have a database and a vault table, we are ready to proceed... */

	printf("Please enter a balance for your test table: ");
	//char *response = get_user_response();

	char input[9];
	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';
	int sanitize_success = sanitize_number(input);

	if (sanitize_success == -1) {
		printf("FATAL ERROR: Invalid entry\n");
		exit(1);
	}

	int input_number = atoll(input);

	create_new_chest(db, "test", input_number, KITCHEN);
	increment_chest_value(db, 2, 500);

	/* Close the database */
	sqlite3_close(db);

	return(0);
}

