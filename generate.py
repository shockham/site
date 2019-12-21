"""Small script to generate a static site from parts"""
from os import listdir
from os.path import isfile, join

SITE_PATH = "./site/"
PAGES_PATH = "./pages/"
PAGES = [f for f in listdir(PAGES_PATH) if isfile(join(PAGES_PATH, f))]


def read_part(part_name, pages, default):
    """Read a part of the site"""
    if part_name in pages:
        pages.remove(part_name)
        with open(join(PAGES_PATH, part_name), "r") as header_file:
            return header_file.read()
    else:
        return default


def gen_navbar(pages):
    """Generate the site navbar """
    navbar = "".join(
        [
            '<a target="_self" href="%s">%s</a>\n' % (s, s.rstrip(".html"))
            for s in pages
        ]
    )
    return navbar + "</div></div><div>"


def main():
    """Main function to build the site"""
    footer_str = read_part("footer.html", PAGES, "</div></body></html>")
    header_str = read_part(
        "header.html", PAGES, "<!DOCTYPE html><head></head><body>"
    ) + gen_navbar(PAGES)

    for page in PAGES:
        with open(join(PAGES_PATH, page), "r") as page_file:
            page_str = header_str + page_file.read() + footer_str
            with open(join(SITE_PATH, page), "w") as site_file:
                site_file.write(page_str)

if __name__ == '__main__':
    main()
