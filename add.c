#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <db.h>
#include <ustr.h>

#include "lib/hdb.h"
#include "types.h"


int hrai_add(const char *db_name, const char *date, int amount, const char *desc)
{
    DBT key, data;
    DB *dbp;

    if (db_create(&dbp, NULL, 0)) return 1;
    if (dbp->open(dbp, NULL, hrai_db_filename(), NULL, DB_BTREE, 0, 0)) return 1;

    memset(&key, 0, sizeof(DBT));
    memset(&data, 0, sizeof(DBT));

    time_t keystamp = time(NULL);
    key.data = &keystamp;
    key.size = sizeof(time_t);

    struct hrai_entry entry;
    memset(&entry, 0, sizeof(struct hrai_entry));
    strptime(date, "%Y-%m-%d", &entry.date);
    entry.amount = amount;
    strncpy(entry.description, desc, 768);
    data.data = &entry;
    data.size = sizeof(struct hrai_entry);

    int ret = dbp->put(dbp, NULL, &key, &data, DB_NOOVERWRITE);

    if (ret == DB_KEYEXIST) return 1;
    if (dbp->close(dbp, 0)) return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    if (hrai_add(hrai_db_filename(), argv[1], atoi(argv[2]), argv[3])) {
        return EXIT_FAILURE;
    } else return EXIT_SUCCESS;
}
