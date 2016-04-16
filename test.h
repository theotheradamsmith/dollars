#ifndef TEST_H
#define TEST_H

/**
 * Receives user input - Numbers
 * @return Returns a sanitized integer
 */
int get_number(void);

/**
 * Receives user input - Strings
 * @return Returns a sanitized string
 */
char *get_string(void);

int test_interface_loop(sqlite3 *db);

#endif /* TEST_H */
