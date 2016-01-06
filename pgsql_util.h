#include <libpq-fe.h>

#include "main.h"

/**
 * Call check_for_database to determine if a database exists; if it does not, create
 * the database. Then connect to the database.
 * @param name The string provided as the database name
 * @param conn_command The 'dbname = x' format connection command used by PQconnectdb
 * @return Returns the PGconn object upon success; else the program will exit
 */
PGconn *initialize_database(const char *name, char *conn_command);

/**
 * Check to see if a database exists.
 * @param name The database name
 * @return Returns true if database exists, else false.
 */
bool check_for_database(const char *name);

/**
 * Used to cleanly terminate a connection to a database upon error.
 * @param conn The database connection object
 */
void exit_nicely(PGconn *conn);

