#include <libexplain/pclose.h>

#include "main.h"
#include "pgsql_util.h"

void initialize_primary_account_table(PGconn *conn);

PGconn *initialize_database(const char *name, char *conn_command) {
	bool exists = check_for_database(name);

	if (!exists) {
		char *create_db_command = malloc(strlen("createdb ") + strlen(name));
		//memset(create_db_command, 0, sizeof(create_db_command));

		printf("Database not found. Creating database...\n");

		create_db_command = strcat(create_db_command, "createdb ");
		create_db_command = strcat(create_db_command, name);

		///@todo replace system call with execve() to prevent exploits
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

	initialize_primary_account_table(conn);

	printf("Database initialization complete.\n");
	
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

void initialize_primary_account_table(PGconn *conn) {
	PGresult *res;
	char *sql = "CREATE TABLE IF NOT EXISTS estate("
				"id         int    PRIMARY KEY    NOT NULL,"
				"name       text                  NOT NULL,"
				"balance    bigint);";

	/* Start transaction block */
	res = PQexec(conn, "BEGIN");
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit_nicely(conn);
	}
	PQclear(res);

	res = PQexec(conn, sql);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "CREATE TABLE command failed: %s", PQerrorMessage(conn));
		PQclear(res);
		exit_nicely(conn);
	}
	PQclear(res);

	/* End the transaction */
	res = PQexec(conn, "END");
	PQclear(res);
}

