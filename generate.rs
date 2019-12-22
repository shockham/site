use std::io;
use std::fs;
use std::fs::File;
use std::io::prelude::*;

const HEADER_HTML: &'static str = include_str!("./pages/header.html");
const FOOTER_HTML: &'static str = include_str!("./pages/footer.html");
const SITE_PATH: &'static str = "./site/";
const PAGES_PATH: &'static str = "./pages/";


fn main() -> io::Result<()> {
    // Generate the nav
    let nav_html = fs::read_dir(PAGES_PATH)?
        .flat_map(|res| res.map(|e| {
            let mut file_name = e.file_name().into_string().unwrap();
            file_name.truncate(file_name.len() - 5);
            println!("{}", file_name);
            if !(file_name == "header" || file_name == "footer") {
                return format!(
                    "<a target=\"_self\" href=\"{}.html\">{}</a>\n", 
                    file_name,
                    file_name
                );
            }
            String::new()
        }))
        .collect::<String>() + "</div></div><div>";
    // Write the pages
    fs::read_dir(PAGES_PATH)?
        .for_each(|res| res.iter().for_each(|e| {
            let path = e.path();
            let path_str: &str = path.to_str().unwrap();
            if !(path_str.contains("header") || path_str.contains("footer")) {
                let mut file = File::open(e.path()).unwrap();
                let mut contents = String::new();
                file.read_to_string(&mut contents).unwrap();

                let html = format!("{}{}{}{}", HEADER_HTML, nav_html, contents, FOOTER_HTML);

                println!("{}", html);
                let file_name = e.file_name().into_string().unwrap();
                let mut file = File::create(format!("{}{}", SITE_PATH, file_name)).unwrap();
                file.write_all(html.as_bytes()).unwrap();
            }
        }));

    Ok(()) 
}
