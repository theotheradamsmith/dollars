#include <gtk/gtk.h>
#include "main.h"
#include "database.h"
#include "string_util.h"
#include "test.h"

char *main_database_err_msg = NULL;

int main(int argc, char *argv[]) {
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

	GtkBuilder *builder;
	GtkWidget *window;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "window_main.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(builder);
	gtk_widget_show(window);
	gtk_main();

	/* Now that we have a database and a vault table, we are ready to proceed...
	if (test_interface_loop(db) != 0) {
		fprintf(stderr, "FATAL ERROR: Interface Loop failure\n");
		sqlite3_close(db);
		exit(1);
	}
	*/

	/* Close the database */
	sqlite3_close(db);

	return(0);
}

void on_window_main_destroy() {
	gtk_main_quit();
}

