#include "main.h"
#include "pgsql_util.h"

int main(int argc, char **argv) {
	const char *conninfo;
	char *conn_cmd_prefix = "dbname = ";
	PGconn *conn;

	if (argc <= 1) {
		printf("Usage error... please provide database name.\n");
		exit(EX_USAGE);
	}
	
	conninfo = argv[1];
	char *conn_command = malloc(strlen(conn_cmd_prefix) + strlen(argv[1]));
	conn_command = strcat(conn_command, "dbname = ");
	conn_command = strcat(conn_command, argv[1]);

	conn = initialize_database(conninfo, conn_command);

	free(conn_command);
	PQfinish(conn);
	return(0);
}

