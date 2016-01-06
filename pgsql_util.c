#include <libexplain/pclose.h>

#include "main.h"
#include "pgsql_util.h"

/**
 * Call check_for_database to determine if a database exists; if it does not, create
 * the database. Then connect to the database.
 * @param name The string provided as the database name
 * @param conn_command The 'dbname = x' format connection command used by PQconnectdb
 * @return Returns the PGconn object upon success; else the program will exit
 */
PGconn *initialize_database(const char *name, char *conn_command) {
	bool exists = check_for_database(name);

	if (!exists) {
		char *create_db_command = malloc(strlen("createdb ") + strlen(name));
		//memset(create_db_command, 0, sizeof(create_db_command));

		printf("Database not found. Creating database...\n");

		create_db_command = strcat(create_db_command, "createdb ");
		create_db_command = strcat(create_db_command, name);

		int system_ret = system(create_db_command);
		sleep(1);
		free(create_db_command);

		if (system_ret < 0) {
			printf("Error creating database. Aborting.\n");
			exit(EX_OSERR);
		} else {
			printf("Database should now exist.\n");
		}
	}

	printf("Attempting to connect to database...\n");

	PGconn *conn = PQconnectdb(conn_command);
	if (PQstatus(conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
		exit_nicely(conn);
	} else {
		printf("Successfully connected to database %s\n", name);
	}
	
	return(conn);
}

bool check_for_database(const char *name) {
	bool res;	
	FILE *fp;
	char *command_prefix = "psql -lqt | cut -d ' ' -f 2 | egrep -w "; 
	char *psql_list = malloc(strlen(command_prefix) + strlen(name));
	char *buf = malloc(1024);

	//memset(psql_list, 0, sizeof(psql_list));
	//memset(buf, 0, sizeof(buf));

	psql_list = strcat(psql_list, command_prefix);
	psql_list = strcat(psql_list, name);

	if ((fp = popen(psql_list, "r")) == NULL) {
		printf("Error opening pipe!\n");
		exit(EX_OSERR);
	}

	if (fgets(buf, sizeof(buf), fp))
		res = true;
	else
		res = false;

	free(buf);
	free(psql_list);

	if (pclose(fp) < 0) {
		printf("Command not found or exited with error status\n");
		fprintf(stderr, "%s\n", explain_pclose(fp));
		exit(EX_OSERR);
	}

	return(res);
}

void exit_nicely(PGconn *conn) {
	PQfinish(conn);
	exit(EX_UNAVAILABLE);
}

