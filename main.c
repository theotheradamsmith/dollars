#include "main.h"
#include "database.h"
#include "string_util.h"
#include "test.h"

char *main_database_err_msg = NULL;

int main(void) {
	char *test_title;
	int test_number, grand_total;
	int test_chest_id;

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

	while (1) {
		printf("\n"\
			   "Options\n"\
			   "1) Display Grand Total\n"\
			   "2) Create a chest\n"\
			   "3) Display Chest Total\n"\
			   "4) Increment Chest\n"\
			   "10) Initialize Uncategorized\n"\
			   "0 to Quit\n"\
			   "\n");
		int selection = get_number();

		switch (selection) {
			case 1 :
				grand_total = calculate_grand_total(db);
				printf("Grand total: %d", grand_total);
				putchar('\n');
				break;
			case 2 :
				printf("Please enter a title for your test table: ");
				test_title = get_string();
				printf("Please enter a balance for your test table: ");
				test_number = get_number();
				create_new_chest(db, test_title, test_number, KITCHEN);
				putchar('\n');
				free(test_title);
				break;
			case 3 :
				printf("Enter name of chest to read: ");
				test_title = get_string();
				test_chest_id = get_chest_id(db, test_title);
				int new_test_balance = read_chest_balance(db, test_chest_id);
				printf("New balance for %s: %d", test_title, new_test_balance);
				putchar('\n');
				free(test_title);
				break;
			case 4 :
				printf("Enter name of chest to increment: ");
				test_title = get_string();
				printf("Enter increment amount: ");
				test_number = get_number();
				test_chest_id = get_chest_id(db, test_title);
				printf("Now I'll increment Chest %d...", test_chest_id);
				increment_chest_value(db, test_chest_id, test_number);
				putchar('\n');
				free(test_title);
				break;
			case 10 :
				printf("Enter a new balance for Uncategorized: ");
				test_number = get_number();
				initialize_uncategorized(db, test_number);
				putchar('\n');
				break;
			default :
				goto terminate;
		}
	}

terminate:
	/* Close the database */
	sqlite3_close(db);

	return(0);
}

