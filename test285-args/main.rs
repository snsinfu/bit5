use std::env;
use std::fmt;
use std::fs;
use std::io;

enum Error {
    Usage,
    IO(io::Error),
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Error::Usage => write!(f, "invalid usage"),
            Error::IO(err) => write!(f, "{}", err),
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if let Err(err) = run(&args) {
        eprintln!("error: {}", err);
        match err {
            Error::Usage => std::process::exit(64),
            _ => std::process::exit(1),
        }
    }
}

fn run(args: &Vec<String>) -> Result<(), Error> {
    match args.as_slice() {
        [_] => run_word_count(&mut io::stdin().lock()),

        [_, filename] => {
            let file = match fs::File::open(filename) {
                Ok(file) => file,
                Err(err) => {
                    return Err(Error::IO(err));
                }
            };
            let mut reader = io::BufReader::new(file);
            run_word_count(&mut reader)
        }

        _ => Err(Error::Usage),
    }
}

fn run_word_count(reader: &mut dyn io::BufRead) -> Result<(), Error> {
    use io::BufRead;

    let mut count: usize = 0;

    for line in reader.lines() {
        match line {
            Ok(line) => {
                count += line.split_whitespace().count();
            }
            Err(err) => {
                return Err(Error::IO(err));
            }
        }
    }

    println!("{}", count);

    Ok(())
}
