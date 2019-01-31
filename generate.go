package main

import (
    "fmt"
    "io/ioutil"
    "path/filepath"
    "strings"
)

func read_part(part_name string, pages []string, def string) (string, []string, error) {
    for i, page := range pages {
        if page == part_name {
            b, err := ioutil.ReadFile(part_name)
            if err != nil {
                return "", pages, err
            }
            s := string(b[:])
            pages = append(pages[:i], pages[i+1:]...)
            return s, pages, nil
        }
    }
    return def, pages, nil
}

func gen_navbar(pages []string) string {
    navbar := ""
    for _, page := range pages {
        page_path := strings.Replace(page, "pages/", "", -1)
        title := strings.Replace(page_path, ".html", "", -1)
        navbar += fmt.Sprintf("<a target=\"_self\" href=\"%s\">%s</a>\n", page_path, title)
    }
    navbar += "</div></div><div>"
    return navbar
}

func main() {
    site_path := "./site/"
    page_path := "./pages/*"

    pages, err := filepath.Glob(page_path)
    if err != nil {
        return
    }

    footer_str, pages, err := read_part("pages/footer.html", pages, "</div></body></html>")
    if err != nil {
        fmt.Println(err)
    }

    header_str, pages, err := read_part("pages/header.html", pages, "<!DOCTYPE html><head></head><body>")
    header_str += gen_navbar(pages)
    if err != nil {
        fmt.Println(err)
    }

    for _, page := range pages {
        b, err := ioutil.ReadFile(page)
        if err != nil {
            return
        }

        page_str := header_str + string(b[:]) + footer_str
        filename := strings.Replace(page, "pages/", "", -1)

        if err = ioutil.WriteFile(site_path + filename, []byte(page_str), 0666); err != nil {
            fmt.Println(err)
        }
    }
}
