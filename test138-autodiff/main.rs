fn main() {
    let y = Mul::new(Sin::new(Var::new()), Cos::new(Var::new()));
    let dy = y.derivative();
    let ddy = dy.derivative();
    let x = 0.7853981633974483;

    println!("x = {}", x);
    println!("y = {}", y.evaluate(x));
    println!("y' = {}", dy.evaluate(x));
    println!("y'' = {}", ddy.evaluate(x));
}

// ---------------------------------------------------------------------------

trait Expression {
    fn evaluate(&self, x: f64) -> f64;
    fn derivative(&self) -> Box<dyn Expression>;
    fn copy(&self) -> Box<dyn Expression>;
}

// ---------------------------------------------------------------------------

struct Constant {
    value: f64,
}

impl Constant {
    fn new(value: f64) -> Box<dyn Expression> {
        Box::new(Constant { value })
    }
}

impl Expression for Constant {
    fn evaluate(&self, _x: f64) -> f64 {
        self.value
    }

    fn derivative(&self) -> Box<dyn Expression> {
        Constant::new(0.0)
    }

    fn copy(&self) -> Box<dyn Expression> {
        Constant::new(self.value)
    }
}

// ---------------------------------------------------------------------------

struct Var {}

impl Var {
    fn new() -> Box<dyn Expression> {
        Box::new(Var {})
    }
}

impl Expression for Var {
    fn evaluate(&self, x: f64) -> f64 {
        x
    }

    fn derivative(&self) -> Box<dyn Expression> {
        Constant::new(1.0)
    }

    fn copy(&self) -> Box<dyn Expression> {
        Var::new()
    }
}

// ---------------------------------------------------------------------------

struct Add {
    left: Box<dyn Expression>,
    right: Box<dyn Expression>,
}

impl Add {
    fn new(left: Box<dyn Expression>, right: Box<dyn Expression>) -> Box<dyn Expression> {
        Box::new(Add { left, right })
    }
}

impl Expression for Add {
    fn evaluate(&self, x: f64) -> f64 {
        self.left.evaluate(x) + self.right.evaluate(x)
    }

    fn derivative(&self) -> Box<dyn Expression> {
        Add::new(self.left.derivative(), self.right.derivative())
    }

    fn copy(&self) -> Box<dyn Expression> {
        Add::new(self.left.copy(), self.right.copy())
    }
}

// ---------------------------------------------------------------------------

struct Mul {
    left: Box<dyn Expression>,
    right: Box<dyn Expression>,
}

impl Mul {
    fn new(left: Box<dyn Expression>, right: Box<dyn Expression>) -> Box<dyn Expression> {
        Box::new(Mul { left, right })
    }
}

impl Expression for Mul {
    fn evaluate(&self, x: f64) -> f64 {
        self.left.evaluate(x) * self.right.evaluate(x)
    }

    fn derivative(&self) -> Box<dyn Expression> {
        Add::new(
            Mul::new(self.left.derivative(), self.right.copy()),
            Mul::new(self.left.copy(), self.right.derivative()),
        )
    }

    fn copy(&self) -> Box<dyn Expression> {
        Mul::new(self.left.copy(), self.right.copy())
    }
}

// ---------------------------------------------------------------------------

struct Sin {
    argument: Box<dyn Expression>,
}

impl Sin {
    fn new(argument: Box<dyn Expression>) -> Box<dyn Expression> {
        Box::new(Sin { argument })
    }
}

struct Cos {
    argument: Box<dyn Expression>,
}

impl Cos {
    fn new(argument: Box<dyn Expression>) -> Box<dyn Expression> {
        Box::new(Cos { argument })
    }
}

impl Expression for Sin {
    fn evaluate(&self, x: f64) -> f64 {
        x.sin()
    }

    fn derivative(&self) -> Box<dyn Expression> {
        Cos::new(self.argument.copy())
    }

    fn copy(&self) -> Box<dyn Expression> {
        Sin::new(self.argument.copy())
    }
}

impl Expression for Cos {
    fn evaluate(&self, x: f64) -> f64 {
        x.cos()
    }

    fn derivative(&self) -> Box<dyn Expression> {
        Mul::new(Constant::new(-1.0), Sin::new(self.argument.copy()))
    }

    fn copy(&self) -> Box<dyn Expression> {
        Cos::new(self.argument.copy())
    }
}
