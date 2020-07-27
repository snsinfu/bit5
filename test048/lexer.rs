use std::iter::Peekable;
use std::str::Chars;

#[derive(Debug)]
pub enum Token {
    Error(&'static str),

    // Keyword-likes
    End,
    Repeat,
    Next,
    Break,
    Identifier(String),

    // Literals
    Number(i64),

    // Operators
    Equal,
    Plus,
    Minus,
    PlusPlus,
    MinusMinus,
    EqualEqual,
    PlusEqual,
    MinusEqual,

    // Punctuation marks
    Colon,
    Newline,
}

pub fn scan(src: &str) -> Vec<Token> {
    let mut chars = src.chars().peekable();
    let mut tokens = Vec::<Token>::new();

    loop {
        let token = scan_token(&mut chars);

        match token {
            Token::Error(_) => break,

            Token::End => {
                tokens.push(token);
                break;
            }

            _ => tokens.push(token),
        }
    }

    tokens
}

fn scan_token(mut chars: &mut Peekable<Chars>) -> Token {
    skip_spaces(&mut chars);

    if let Some(ch) = chars.peek() {
        match ch {
            'A'..='Z' | 'a'..='z' => scan_keyword_like(&mut chars),

            '0'..='9' => scan_number(&mut chars),

            '=' => scan_equal_like(&mut chars),
            '+' => scan_plus_like(&mut chars),
            '-' => scan_minus_like(&mut chars),
            ',' => scan_colon_like(&mut chars),
            '\n' => scan_newline_like(&mut chars),

            _ => Token::Error("unrecognized character"),
        }
    } else {
        Token::End
    }
}

fn skip_spaces(chars: &mut Peekable<Chars>) {
    while let Some(ch) = chars.peek() {
        if *ch == '\n' || !ch.is_ascii_whitespace() {
            break;
        }
        chars.next();
    }
}

fn scan_keyword_like(chars: &mut Peekable<Chars>) -> Token {
    let mut name = String::new();

    while let Some(ch) = chars.peek() {
        if ch.is_ascii_alphabetic() {
            name.push(*ch);
            chars.next();
        } else {
            break;
        }
    }

    match name.as_str() {
        "end" => Token::End,
        "repeat" => Token::Repeat,
        "next" => Token::Next,
        "break" => Token::Break,
        _ => Token::Identifier(name),
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

fn scan_equal_like(chars: &mut Peekable<Chars>) -> Token {
    chars.next(); // Consume leading '='

    match chars.peek() {
        Some('=') => {
            chars.next();
            Token::EqualEqual
        }

        _ => Token::Equal,
    }
}

fn scan_plus_like(chars: &mut Peekable<Chars>) -> Token {
    chars.next(); // Consume leading '+'

    match chars.peek() {
        Some('=') => {
            chars.next();
            Token::PlusEqual
        }

        Some('+') => {
            chars.next();
            Token::PlusPlus
        }

        _ => Token::Plus,
    }
}

fn scan_minus_like(chars: &mut Peekable<Chars>) -> Token {
    chars.next(); // Consume leading '-'

    match chars.peek() {
        Some('=') => {
            chars.next();
            Token::MinusEqual
        }

        Some('-') => {
            chars.next();
            Token::MinusMinus
        }

        _ => Token::Minus,
    }
}

fn scan_colon_like(chars: &mut Peekable<Chars>) -> Token {
    chars.next(); // Consume leading ','
    Token::Colon
}

fn scan_newline_like(mut chars: &mut Peekable<Chars>) -> Token {
    chars.next(); // Consume leading '\n'

    while chars.peek() == Some(&'\n') {
        chars.next();
        skip_spaces(&mut chars);
    }

    Token::Newline
}
