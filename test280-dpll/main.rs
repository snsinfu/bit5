type Formula = Vec::<Clause>;
type Clause = Vec::<Literal>;

#[derive(Clone, Copy, Debug)]
enum Literal {
    Var(usize),
    Not(usize),
}

use Literal::Var;
use Literal::Not;

fn main() {
    let a: usize = 0;
    let b: usize = 1;
    let c: usize = 2;

    let formula: Formula = vec![
        vec![Var(a), Var(b), Var(c)],
        vec![Not(a), Not(b)],
        vec![Not(b), Not(c)],
        vec![Not(c), Not(a)],
    ];

    let mut vars = Vec::<bool>::new();
    vars.resize(3, false);

    if check_sat(&formula, &mut vars) {
        println!("sat {:?}", vars);
    } else {
        println!("unsat");
    }
}

fn check_sat(formula: &Formula, mut vars: &mut Vec::<bool>) -> bool {
    // Stop conditions
    if eval_formula(&formula, &mut vars) {
        return true;
    }

    for clause in formula {
        if clause.is_empty() {
            return false;
        }
    }

    // TODO: Unit propagation
    // TODO: Pure literal elimination

    // Splitting
    let i = match &formula[0][0] {
        &Var(i) => i,
        &Not(i) => i,
    };

    vars[i] = true;
    if check_sat(&assign(formula, i, true), &mut vars) {
        return true;
    }

    vars[i] = false;
    if check_sat(&assign(formula, i, false), &mut vars) {
        return true;
    }

    return false;
}

fn assign(formula: &Formula, var: usize, value: bool) -> Formula {
    let mut new_formula = Formula::new();

    'formula: for clause in formula {
        let mut new_clause = Clause::new();

        for lit in clause {
            let (i, not) = match lit {
                &Var(i) => (i, false),
                &Not(i) => (i, true),
            };
            if i == var {
                if value == not {
                    continue;
                } else {
                    continue 'formula;
                }
            } else {
                new_clause.push(*lit);
            }
        }

        new_formula.push(new_clause);
    }

    return new_formula;
}

fn eval_formula(formula: &Formula, vars: &Vec::<bool>) -> bool {
    for clause in formula {
        if !eval_clause(&clause, &vars) {
            return false;
        }
    }
    return true;
}

fn eval_clause(clause: &Clause, vars: &Vec::<bool>) -> bool {
    for lit in clause {
        let cond = match lit {
            &Var(i) => vars[i],
            &Not(i) => !vars[i],
        };
        if cond {
            return true;
        }
    }
    return false;
}
