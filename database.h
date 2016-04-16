#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"

/**
 * Establishes the root account upon initial use.
 * @param database The database the program has connected to
 * @return Returns SQLITE_OK upon success, otherwise passes an error message
 */
int initialize_vault(sqlite3 *database);

/**
 * Inserts a new row into the vault table to represent a new account
 * @param database The database the program has connected to
 * @param name A string representing the new account name
 * @param balance An integer representation of the account balance in pennies
 * @param family The classification scheme assigned to the account
 * @return Returns 0 upon success
 */
int create_new_chest(sqlite3 *database, char *name, int balance, int family);

/**
 * Updates the balance of a chest that is identified by ID
 * @param database The database the program has connected to
 * @param id The integer ID of the chest
 * @param balance The new dollar amount to assign to the chest (in pennies)
 * @return Returns 0 upon success, -1 if there is an error
 */
int update_chest_balance(sqlite3 *database, int id, int balance);

/**
 * Increments a table by a user-specified value. Retrieve the current balance of
 * a chest, add the increment amount, and call update_chest_balance with the new
 * balance value.
 * @param database The database the program has connected to
 * @param id The integer ID of the chest
 * @param increment_amount The dollar amount by which to increment the balance
 * @return 0 on success, -1 if there is an error
 */
int increment_chest_value(sqlite3 *database, int id, int increment_amount);

/**
 * Reports the current balance of a specified chest
 * @param database The database the program has connected to
 * @param id The integer ID of the chest
 * @return Returns the chest balance as an integer, exits on error
 */
int read_chest_balance(sqlite3 *database, int id);

/**
 * Returns the corresponding integer id for a user-named chest
 * @param database The database the program has connected to
 * @param name The user-specified name of the chest
 * @return Returns the integer id of a specified chest, -1 on error
 */
int get_chest_id(sqlite3 *database, char *name);

int calculate_grand_total(sqlite3 *database);

int initialize_uncategorized(sqlite3 *database, int balance);

int decrement_uncategorized_balance(sqlite3 *database, int decrement_amount);
#endif /* DATABASE_H */
