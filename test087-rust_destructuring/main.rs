fn main() {
    // Intentionally declaring variables in different order as that of the
    // Options fields. It works as expected; the binding is name-based.
    let Options {
        mut filename, // Can be `mut`
        verbose,
    } = get_options();

    if !filename.ends_with(".txt") {
        filename = filename + ".txt";
    }

    println!("verbose {:}", verbose);
    println!("filename {:}", filename);
}

struct Options {
    verbose: bool,
    filename: String,
}

fn get_options() -> Options {
    let opts = Options {
        verbose: false,
        filename: "foobar".to_string(),
    };

    // ...

    opts
}
