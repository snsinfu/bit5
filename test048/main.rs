mod lexer;
mod parser;

use std::fs::File;
use std::io;
use std::io::Read;

fn main() {
    let filename = if let Some(filename) = single(std::env::args().skip(1)) {
        filename
    } else {
        eprintln!("usage: main <filename>");
        std::process::exit(1);
    };

    if let Err(err) = do_main(&filename) {
        eprintln!("error: {}", err);
        std::process::exit(1);
    }
}

fn single<I: Iterator>(mut it: I) -> Option<I::Item> {
    match (it.next(), it.next()) {
        (Some(value), None) => Some(value),
        _ => None,
    }
}

fn do_main(filename: &str) -> Result<(), io::Error> {
    let mut file = File::open(filename)?;
    let mut source = String::new();

    file.read_to_string(&mut source)?;

    let tokens = lexer::scan(&source);
    println!("{:?}", tokens);

    let tree = parser::parse(&tokens);
    println!("{:?}", tree);

    Ok(())
}
