// Knapsack problem. 2D cost, uniform reward.

use std::collections::HashMap;

#[derive(Hash, PartialEq, Eq, Debug)]
struct Cost(i32, i32);

#[derive(Hash, PartialEq, Eq, Debug)]
struct State {
    space: Cost,
    reward: i32,
}

type Memo = HashMap<State, i32>;

fn main() {

    let items = vec![
        Cost(2, 3), Cost(1, 4), Cost(3, 1), Cost(4, 2)
    ];
    let init = State {
        space: Cost(40, 50),
        reward: 0,
    };

    let mut memo = Memo::new();

    let reward = knapsack(&items, &mut memo, init);
    println!("{}", reward);
}

fn knapsack(items: &Vec<Cost>, mut memo: &mut Memo, state: State) -> i32 {
    if let Some(result) = memo.get(&state) {
        return *result;
    }

    let mut best: i32 = 0;

    for item_cost in items {
        let space = Cost(state.space.0 - item_cost.0, state.space.1 - item_cost.1);
        if space.0 < 0 || space.1 < 0 {
            continue;
        }

        let next_state = State {
            space: space,
            reward: state.reward + 1,
        };

        let reward = 1 + knapsack(&items, &mut memo, next_state);

        if reward > best {
            best = reward
        }
    }

    // Print the search tree. Memoization greatly reduces the number of leaves.
    // Try commenting out the `memo.insert` line.
    println!("{}\t{:?}", best, state);
    memo.insert(state, best);

    return best;
}
