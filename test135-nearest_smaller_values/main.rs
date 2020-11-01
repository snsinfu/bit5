fn main() {
    let seq = vec!(0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15);

    let mut stack = Vec::<i32>::new();

    for value in seq {
        while let Some(&top) = stack.last() {
            if top < value {
                break;
            }
            stack.pop();
        }

        if let Some(&top) = stack.last() {
            println!("{}", top)
        } else {
            println!("*")
        }

        stack.push(value);
    }
}
