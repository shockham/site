#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

static void output_file (const char *fn);
static void gen_pages ();
static void gen_nav ();

const char* pages_path = "./pages/";

int main (void) {
    gen_nav();

    return 0;
}


static void gen_pages () {
    DIR* dp;
    struct dirent *ep;

    dp = opendir (pages_path);
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            if (ep->d_type == DT_REG) {
                printf ("Processing: %s\n", ep->d_name);
                output_file (ep->d_name);
            }
        }

        (void) closedir (dp);
    } else {
        perror ("Couldn't open the directory");
    }
}

static void gen_nav () {
    DIR* dp;
    struct dirent *ep;

    dp = opendir (pages_path);
    if (dp != NULL) {
        char buffer[4096];
        while (ep = readdir (dp)) {
            if (ep->d_type == DT_REG) {
                strcat (buffer, "<a target=\"_self\" href=\"");
                strcat (buffer, ep->d_name);
                strcat (buffer, ".html\">");
                strcat (buffer, ep->d_name);
                strcat (buffer, "</a>");
            }
        }

        printf ("Nav: %s\n", buffer);

        (void) closedir (dp);
    } else {
        perror ("Couldn't open the directory");
    }
}

static void output_file (const char *fn) {
    FILE* fp;

    char full_path[64];
    strcpy (full_path, pages_path);
    strcat (full_path, fn);

    fp = fopen (full_path, "r");
    if (fp != NULL) {
        size_t n;
        char buffer[4096];

        do {
            n = fread (buffer, 1, 4096, fp);
            fwrite (buffer, 1, n, stdout);
        } while (n != 0);

        fclose (fp);
    } else {
        perror ("Couldn't open file");
    }
}
