fn main() {
    //     o 4
    //    / \
    // 0 o---o 1
    //   |
    // 2 o---o 3
    let edges: Vec<(usize, usize)> = vec![(0, 1), (0, 2), (0, 4), (1, 4), (2, 3)];
    let target = 1;

    // Number of pebbles on each vertex.
    let mut pebbles: Vec<i32> = vec![0, 0, 0, 11, 0];
    let mut step = 0;

    println!("INIT {:?}", pebbles);

    while pebbles[target] == 0 {
        let mut ok = false;

        for (i, &pebble) in pebbles.iter().enumerate() {
            if pebble >= 2 {
                let j = choose_adjacent(&edges, i, step);
                pebbles[i] -= 2;
                pebbles[j] += 1;
                ok = true;
                break;
            }
        }

        if !ok {
            // Failed.
            break;
        }

        step += 1;

        println!("{:} {:?}", step, pebbles);
    }

    if pebbles[target] != 0 {
        println!("OK");
    } else {
        println!("FAILED");
    }
}

fn choose_adjacent(edges: &Vec<(usize, usize)>, vertex: usize, seed: i32) -> usize {
    // Ugly allocating algorithm! Here adjacency list is created EACH TIME the
    // function is called. I should have used adjacency list representation in
    // the first place instead of edge list.
    let mut adjacents = Vec::<usize>::new();

    for &(i, j) in edges {
        if i == vertex {
            adjacents.push(j);
        }
        if j == vertex {
            adjacents.push(i);
        }
    }

    // Use seed (arbitrary integral value) is used to choose adjacent vertex
    // pseudo-randomly. Rust does not have RNG in the stdlib...
    adjacents[(seed as usize) % adjacents.len()]
}
