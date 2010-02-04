#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <db.h>
#include <ustr.h>

#include "lib/hdb.h"
#include "types.h"


int hrai_list(const char *db_name)
{
    DBT key, data;
    DBC *it;
    DB *dbp;

    if (db_create(&dbp, NULL, 0)) return 1;
    if (dbp->open(dbp, NULL, hrai_db_filename(), NULL, DB_BTREE, 0, 0)) return 1;

    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));

    dbp->cursor(dbp, NULL, &it, 0);

    int ret;
    while ((ret = it->get(it, &key, &data, DB_NEXT)) == 0) {
        struct hrai_entry *entry = (struct hrai_entry *)data.data;
        time_t *keystamp = (time_t *)key.data;

        printf("%ld: %4d-%02d-%02d [%u] %s\n", *keystamp
                                             , (entry->date).tm_year + 1900
                                             , (entry->date).tm_mon + 1
                                             , (entry->date).tm_mday
                                             , entry->amount
                                             , entry->description
                                             );
    }

    if (ret != DB_NOTFOUND) return 1;
    if (it != NULL) it->close(it);
    if (dbp->close(dbp, 0)) return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    return hrai_list(hrai_db_filename()) ? EXIT_FAILURE : EXIT_SUCCESS;
}
