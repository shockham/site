from os import listdir
from os.path import isfile, join

site_path = "./site/"
pages_path = "./pages/"
pages = [f for f in listdir(pages_path) if isfile(join(pages_path, f))]


def read_part(part_name, pages, default):
    if part_name in pages:
        pages.remove(part_name)
        with open(join(pages_path, part_name), "r") as header_file:
            return header_file.read()
    else:
        return default


def gen_navbar(pages):
    navbar = "".join(
        ['<a target="_self" href="%s">%s</a>\n' % (s, s.rstrip(".html")) for s in pages]
    )
    return navbar + "</div></div><div>"


footer_str = read_part("footer.html", pages, "</div></body></html>")
header_str = read_part(
    "header.html", pages, "<!DOCTYPE html><head></head><body>"
) + gen_navbar(pages)

for p in pages:
    with open(join(pages_path, p), "r") as page_file:
        page_str = header_str + page_file.read() + footer_str
        with open(join(site_path, p), "w") as site_file:
            site_file.write(page_str)
