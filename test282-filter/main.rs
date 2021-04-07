fn main() {
    let mut seq = vec![-1, 2, -3, 4, -5, 6];

    while !seq.is_empty() {
        // Remove a "tag" element that satisfies a certain condition.
        let mut tag: Option<i32> = None;

        for (i, &elem) in seq.iter().enumerate() {
            if elem < 0 {
                tag = Some(seq.remove(i));
                break;
            }
        }

        // Modify the sequence using removed value.
        if let Some(val) = tag {
            for elem in seq.iter_mut() {
                *elem -= val;
            }
            println!("Removed: {} -> {:?}", val, seq);
        } else {
            break;
        }
    }
}
