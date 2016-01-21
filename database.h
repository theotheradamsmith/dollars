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
