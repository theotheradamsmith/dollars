#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "string_util.h"
#include "database.h"

char *categories[] = {"ROOT", "KITCHEN", "AUTOMOTIVE", "LAUNDRY"};

int get_number(void) {
	char input[9];
	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';

	if (sanitize_number(input) == -1) {
		printf("FATAL ERROR: Invalid entry\n");
		exit(1);
	}

	return(atoll(input));
}

char *get_string(void) {
	char input[50];
	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';

	sanitize_string(input);

	char *output = strdup(input);

	return(output);
}

int test_interface_loop(sqlite3 *db) {
	char *test_title, *test_category;
	int test_number, grand_total, test_chest_id;
	int test_category_number = 0;
	int num_cats = sizeof(categories)/sizeof(categories[0]);

	while (1) {
		printf("\n"\
			   "Options\n"\
			   "1) Display Grand Total\n"\
			   "2) Create a chest\n"\
			   "3) Display Chest Total\n"\
			   "4) Increment Chest\n"\
			   "10) Initialize Uncategorized\n"\
			   "11) Print Categories\n"\
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
				printf("Please enter a category for your test table: ");
				test_category = get_string();
				for (int i = 0; i < num_cats; i++) {
					if (!strcmp(test_category, categories[i])) {
						test_category_number = i;
					}
				}
				create_new_chest(db, test_title, test_number, test_category_number);
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
			case 11 :
				for (int i = 0; i < num_cats; i++) {
					printf("Category: %s\n", categories[i]);
				}
				break;
			default :
				goto terminate;
		}
	}
terminate:
	return(0);
}
