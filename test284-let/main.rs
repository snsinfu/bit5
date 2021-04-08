fn main() {
    let xs = vec![1, -2, 3, -4, 5, -6];
    doit(&xs);
}

fn doit(xs: &Vec::<i32>) {
    let mut xs = xs.clone();
    let mut i = 0;

    while i < xs.len() {
        if xs[i] < 0 {
            let n = xs.len();
            xs.swap(i, n - 1);
            xs.pop();
            continue;
        }
        i += 1;
    }

    println!("{:?}", xs);
}
