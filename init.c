#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <db.h>

#include "lib/hdb.h"


int hrai_init(const char *dbname, int force)
{
    DB *dbp;

    if (db_create(&dbp, NULL, 0)) return 1;
    if (dbp->open(dbp, NULL, hrai_db_filename()
                     , NULL, DB_BTREE, DB_CREATE | DB_EXCL, 0)) {
         return 1;
    }
    if (dbp->close(dbp, 0)) return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    return hrai_init(hrai_db_filename(), 1) ? EXIT_FAILURE : EXIT_SUCCESS;
}
