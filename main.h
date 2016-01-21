#include <stdio.h>
#include <stdlib.h>

#include "sqlite3.h"

// Database Name
#define ROOT_DB "estate.db"

// Chest Categories, or "Families"
enum {
	ROOT,
	KITCHEN,
	AUTOMOTIVE,
	LAUNDRY,
};

