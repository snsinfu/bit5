use std::iter::Peekable;
use std::slice::Iter;

use lexer::Token;


pub fn parse(tokens: &Vec<Token>) {
    parse_program(&mut tokens.iter().peekable());
}

fn parse_program(tokens: &mut Peekable<Iter<Token>>) {
    todo!()
}
