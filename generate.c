#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

static void output_file (const char *fn);

const char* pages_path = "./pages/";

int main (void) {
    DIR *dp;
    struct dirent *ep;

    dp = opendir (pages_path);
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            if (ep->d_type == DT_REG) {
                printf ("reg: %s\n", ep->d_name);
                output_file (ep->d_name);
            }
        }

        (void) closedir (dp);
    } else {
        perror ("Couldn't open the directory");
    }

    return 0;
}

static void output_file (const char *fn) {
    FILE *fp;

    char full_path[64];
    strcpy (full_path, pages_path);
    strcat (full_path, fn);

    fp = fopen (full_path, "r");
    if (fp != NULL) {
        size_t n;
        char buffer[4096];

        do {
            /* Read some bytes from file */
            n = fread (buffer, 1, 4096, fp);
            /* Output bytes to screen */
            fwrite (buffer, 1, n, stdout);
        } while (n != 0);

        fclose (fp);
    } else {
        perror ("Couldn't open file");
    }
}
