mod mo;

fn main() {
    let cond: mo::Cond = mo::check(-10);

    match cond {
        mo::Cond::Unassigned => std::process::exit(1),
        mo::Cond::Assigned(v) => std::process::exit(if v { 0 } else { 1 }),
    }
}
