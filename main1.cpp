//#include <bits/stdc++.h>

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "list.h"
#include "tree.h"
#include "database2.h"
#define LEN 890

int solve(database2 &, const char*);

int main (int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <input file> <hash value>\n", argv[0]);
        return 1;
    }

    const char *atxt = argv[1];
    int p = atoi(argv[2]);
    if (p <= 0) { printf("Bad hash value\n"); return 2; }

    database2 db2;
    db2.set_p(p);
    //printf("Start:\n");
    //s.print();

    clock_t time;
    time = clock();
    int answer = solve(db2, atxt);
    time = clock() - time;

    //destruct db2??

    if (answer < 0)
    {
        switch (answer)
        {
        case -4:
            printf("A.txt open error\n");
            break;
        case -5:
            printf("Read error in %s\n", atxt);
            break;
        default:
            printf("Unknown error %d\n", answer);
        }
        return 1;
    }


    printf("Worktime = %.2lf\n", (double) time / CLOCKS_PER_SEC);

    return 0;
}

int solve(database2 &db2, const char *atxt)
{
    FILE *aopen;
    char nbuf[LEN], buf[LEN];

    if (!(aopen = fopen(atxt, "r"))) { return -4; }
    //int ret_read = li.read_infinite(aopen);
    int ret_read = db2.read(aopen);
    if (ret_read) { fclose(aopen); return -5; }
    //tr.print_tree();
    fclose(aopen);

    //db2.print();

    for (int i = 0; i < LEN; i++)
        nbuf[i] = 0;

    int spaces[257];
    for (int i = 0; i < 256; i++)
        spaces[i] = 0;

    spaces[(unsigned int)'('] =
    spaces[(unsigned int)')'] =
    spaces[(unsigned int)' '] =
    spaces[(unsigned int)'\t'] =
    spaces[(unsigned int)'\n'] =
    spaces[(unsigned int)';'] =
    spaces[(unsigned int)','] = 1;



    //tr.delete_tree(); li.delete_list();
    parse vhod; command cmd;
    vhod.init_words();
    //cmd.init_table();

    //db2.how_good_is_hash();

    //cmd.print_hat();
    while(fgets(buf, LEN, stdin))
    {
        for (int j = 0; buf[j]; j++)
            if (buf[j] == ';') { buf[j] = 0; break; }

        vhod.init(nbuf, buf, spaces);
        cmd.parser(vhod);

        if (!db2.operation(cmd)) break;

        cmd.nullify();
    }
    cmd.nullify();
    //cmd.delete_table();
    //printf("\nafter:\n"); db2.print();

    return 0;
}
