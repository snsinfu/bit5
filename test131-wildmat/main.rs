fn main() {
    assert!(wildmat("test131-wildmat", "test*-wildmat"));
    assert!(wildmat("test131-wildmat", "test131-*"));
    assert!(!wildmat("test129-tmux", "test*-wildmat"));
    assert!(!wildmat("test129-tmux", "test131-*"));
}

fn wildmat(text: &str, pattern: &str) -> bool {
    println!("'{}' '{}'", text, pattern);

    if pattern.is_empty() {
        return text.is_empty();
    }

    let (pat, pattern_rest) = trim_first(pattern).unwrap();

    if pat == '*' {
        let mut text_track = text;

        loop {
            if wildmat(text_track, pattern_rest) {
                return true;
            }
            if let Some((_, text_rest)) = trim_first(text_track) {
                text_track = text_rest;
            } else {
                break;
            }
        }
        return false;
    }

    if let Some((ch, text_rest)) = trim_first(text) {
        return ch == pat && wildmat(text_rest, pattern_rest);
    }
    return false;
}

fn trim_first(text: &str) -> Option<(char, &str)> {
    if let Some(ch) = text.chars().next() {
        let size = ch.len_utf8();
        return Some((ch, &text[size..]))
    }
    None
}
