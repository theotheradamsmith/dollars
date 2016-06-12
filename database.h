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

/**
 * Calculates the total balance of all accounts by summing individual chest balances
 * and adding these to the uncategorized balance.
 * @param database The database the program has connected to
 * @return Returns the grand total as an integer value
 */
int calculate_grand_total(sqlite3 *database);

/**
 * Sets the uncategorized balance to a user-specified value.
 * Should only be run once.
 * @param database The database the program has connected to
 * @param balance The integer value to set the balance to
 * @return Returns 0 on success, -1 on failure
 */
int initialize_uncategorized(sqlite3 *database, int balance);

/**
 * Alters the uncategorized balance by the value specified by decrement_amount. Should
 * be called by functions that add/remove funds from the uncategorized chest so that
 * grand total remains static.
 * @param database The database the program has connected to
 * @param decrement_amount The amount to subtract from the current uncategorized balance
 * @return Returns 0 on success, -1 failure
 */
int decrement_uncategorized_balance(sqlite3 *database, int decrement_amount);

/**
 * Prints the Name, Balance, and Family Category of a specified chest.
 * @param database The database the program has connected to
 * @param chest_name The name of the chest to print
 * @return Returns 0 on success, -1 on failure
 */
int print_chest(sqlite3 *database, char *chest_name);

int print_table(sqlite3 *db);

#endif /* DATABASE_H */
