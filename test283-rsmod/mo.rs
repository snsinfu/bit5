#[derive(PartialEq)]
pub enum Cond {
    Unassigned,
    Assigned(bool),
}

pub fn check(n: i32) -> Cond {
    if n == 0 {
        return Cond::Unassigned;
    }
    return Cond::Assigned(assign(n));
}

#[test]
fn test_check() {
    assert!(check(0) == Cond::Unassigned);
    assert!(check(-1) == Cond::Assigned(false));
    assert!(check(1) == Cond::Assigned(true));
}

// Just to see how a private function in a module works.
fn assign(n: i32) -> bool {
    return n > 0;
}

#[test]
fn test_assign() {
    assert!(assign(1));
    assert!(!assign(-1));
}
