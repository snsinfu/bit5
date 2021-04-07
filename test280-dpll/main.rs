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

fn check_sat(given_formula: &Formula, mut vars: &mut Vec::<bool>) -> bool {
    let mut formula = given_formula.clone();

    unit_prop(&mut formula, &mut vars);

    if formula.is_empty() {
        return true;
    }

    for clause in formula.iter() {
        if clause.is_empty() {
            return false;
        }
    }

    // Splitting
    let i = match &formula[0][0] {
        &Var(i) => i,
        &Not(i) => i,
    };

    formula.push(vec![Var(i)]);
    if check_sat(&formula, &mut vars) {
        return true;
    }

    formula.pop();
    formula.push(vec![Not(i)]);
    if check_sat(&formula, &mut vars) {
        return true;
    }

    return false;
}

fn unit_prop(formula: &mut Formula, vars: &mut Vec::<bool>) {
    while !formula.is_empty() {
        let mut unit_clause: Option::<Literal> = None;

        for (i, clause) in formula.iter().enumerate() {
            if clause.len() == 1 {
                unit_clause = Some(clause[0]);
                formula.remove(i);
                break;
            }
        }

        if let Some(lit) = unit_clause {
            *formula = match lit {
                Var(i) => {
                    vars[i] = true;
                    assign(&formula, i, true)
                },
                Not(i) => {
                    vars[i] = false;
                    assign(&formula, i, false)
                },
            }
        } else {
            break;
        }
    }
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
