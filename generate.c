#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

static void output_file (const char* fn, char buffer[4096]);
static void gen_pages (char nav_html[4096]);
static void gen_nav (char buffer[4096]);

const char* header_path = "header.html";
const char* footer_path = "footer.html";
const char* pages_path = "./pages/";

int main (void) {
    char nav_html[4096];
    gen_nav(nav_html);

    gen_pages(nav_html);

    return 0;
}

static void gen_pages (char nav_html[4096]) {
    DIR* dp;
    struct dirent *ep;

    char header_html[4096];
    output_file (header_path, header_html);

    char footer_html[4096];
    output_file (footer_path, footer_html);

    dp = opendir (pages_path);
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            if (
                ep->d_type == DT_REG &&
                strcmp (ep->d_name, header_path) &&
                strcmp (ep->d_name, footer_path)
            ) {
                char page_html[4096];
                output_file (ep->d_name, page_html);

                char buffer[4096];
                strcat (buffer, header_html);
                strcat (buffer, nav_html);
                strcat (buffer, page_html);
                strcat (buffer, footer_html);

                printf ("Page %s: %s\n\n", ep->d_name, buffer);

                memset(&buffer[0], 0, sizeof(buffer));
            }
        }

        (void) closedir (dp);
    } else {
        perror ("Couldn't open the directory");
    }
}

static void gen_nav (char buffer[4096]) {
    DIR* dp;
    struct dirent *ep;

    dp = opendir (pages_path);
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            if (ep->d_type == DT_REG) {
                strcat (buffer, "<a target=\"_self\" href=\"");
                strcat (buffer, ep->d_name);
                strcat (buffer, ".html\">");
                strcat (buffer, ep->d_name);
                strcat (buffer, "</a>\n");
            }
        }

        (void) closedir (dp);
    } else {
        perror ("Couldn't open the directory");
    }
}

static void output_file (const char *fn, char buffer[4096]) {
    FILE* fp;

    char full_path[64];
    strcpy (full_path, pages_path);
    strcat (full_path, fn);

    fp = fopen (full_path, "r");
    if (fp != NULL) {
        size_t n;

        do {
            n = fread (buffer, 1, 4096, fp);
        } while (n != 0);

        fclose (fp);
    } else {
        perror ("Couldn't open file");
    }
}
