fn main() {
    let s: &str = "world";
    let r: String = build_string();

    greet(&s);
    greet(&r);
}

fn greet(s: &str) {
    println!("Hello, {}!", s);
}

fn build_string() -> String {
    let mut s = String::new();
    s += "s";
    s += "t";
    s += "r";
    s += "i";
    s += "n";
    s += "g";

    s
}
