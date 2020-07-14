use std::iter::Peekable;
use std::str::Chars;

#[derive(Debug)]
enum Token {
    Number(i64),
    Plus,
    Minus,
    Asterisk,
    Slash,
    LParen,
    RParen,
    Error(&'static str),
    End,
}

fn main() {
    scan(" 123 + (45 - 6) * -7 / 890 ");
}

fn scan(src: &str) {
    let mut chars = src.chars().peekable();
    let mut tokens = Vec::<Token>::new();

    loop {
        let token = scan_token(&mut chars);

        println!("{:?}", token);

        match token {
            Token::End => break,
            Token::Error(_) => break,
            _ => tokens.push(token),
        }
    }
}

fn scan_token(mut chars: &mut Peekable<Chars>) -> Token {
    skip_spaces(&mut chars);

    if let Some(ch) = chars.peek() {
        match ch {
            '0'..='9' => scan_number(&mut chars),

            '+' => {
                chars.next();
                Token::Plus
            }

            '-' => {
                chars.next();
                Token::Minus
            }

            '*' => {
                chars.next();
                Token::Asterisk
            }

            '/' => {
                chars.next();
                Token::Slash
            }

            '(' => {
                chars.next();
                Token::LParen
            }

            ')' => {
                chars.next();
                Token::RParen
            }

            _ => Token::Error("unrecognized char"),
        }
    } else {
        Token::End
    }
}

fn scan_number(chars: &mut Peekable<Chars>) -> Token {
    let mut value: i64 = 0;

    while let Some(ch) = chars.peek() {
        if ch.is_ascii_digit() {
            value *= 10;
            value += ch.to_digit(10).expect("digit") as i64;
            chars.next();
        } else {
            break;
        }
    }

    Token::Number(value)
}

fn skip_spaces(chars: &mut Peekable<Chars>) {
    while let Some(ch) = chars.peek() {
        if !ch.is_ascii_whitespace() {
            break;
        }
        chars.next();
    }
}
