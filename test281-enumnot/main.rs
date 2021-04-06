type Variable = i32;

enum Literal {
    Var(Variable),
    Not(Variable),
}

use Literal::Var;
use Literal::Not;

fn main() {
    let x = Not(2);

    match x {
        Var(i) => println!("{}", i),
        Not(i) => println!("~{}", i),
    }
}
