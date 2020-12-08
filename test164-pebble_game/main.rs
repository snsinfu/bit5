fn main() {
    //     o 4
    //    / \
    // 0 o---o 1
    //   |
    // 2 o---o 3
    let edges: Vec<(usize, usize)> = vec![(0, 1), (0, 2), (0, 4), (1, 4), (2, 3)];
    let adjacents = make_adjacent_lists(&edges, 5);
    let target = 1;

    // Number of pebbles on each vertex.
    let mut pebbles: Vec<i32> = vec![0, 0, 0, 11, 0];
    let mut step = 0;

    println!("INIT {:?}", pebbles);

    while pebbles[target] == 0 {
        let mut ok = false;

        for (i, &pebble) in pebbles.iter().enumerate() {
            if pebble >= 2 {
                let j = choose_adjacent(&adjacents[i], step);
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

fn make_adjacent_lists(edges: &Vec<(usize, usize)>, vertices: usize) -> Vec<Vec<usize>> {
    let mut lists: Vec<Vec<usize>> = Vec::new();

    lists.resize(vertices, vec![]);

    for &(i, j) in edges {
        lists[i].push(j);
        lists[j].push(i);
    }

    lists
}

fn choose_adjacent(adjacents: &Vec<usize>, step: i32) -> usize {
    // Rust does not have RNG in the stdlib. Here I use game step as poor-man's
    // pseudo-random value...
    adjacents[(step as usize) % adjacents.len()]
}
