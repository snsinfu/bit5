use argparse::{ArgumentParser, Store, StoreTrue};

fn main() {
    let mut repeat = 1;
    let mut verbose = false;
    let mut name = "world".to_string();
    {
        let mut parser = ArgumentParser::new();

        parser.set_description("Print greeting message.");

        parser.refer(&mut repeat).add_option(
            &["-n", "--repeat"],
            Store,
            "Repeat greeting this number of times.",
        );

        parser.refer(&mut verbose).add_option(
            &["-v", "--verbose"],
            StoreTrue,
            "Enable verbose logging.",
        );

        parser
            .refer(&mut name)
            .add_argument("name", Store, "Specify name to greet.");

        parser.parse_args_or_exit();
    }

    if verbose {
        eprintln!("[V] Name is '{:}'", name);
        eprintln!("[V] Repeat {:} times", repeat);
    }

    for i in 0..repeat {
        if verbose {
            eprintln!("[V] Output number {:}/{:}", i + 1, repeat);
        }
        println!("Hello, {:}!", name);
    }
}
