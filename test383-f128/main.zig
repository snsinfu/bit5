const std = @import("std");
const math = std.math;
const print = std.debug.print;


pub fn main() void {

    // binary128 should be able to represent exp(-3210) but my implementation
    // of exp(f128) does not work...

    var logits = [_]f128{-1, -23, -456, -3210};
    var sum_exp: f128 = 0;

    for (logits) |logit| {
        const term = exp(logit);
        print("{}\n", .{term});
        sum_exp += term;
    }

    print("{}\n", .{sum_exp});
}


// I can't use std.math.exp.
// "error: exp not implemented for f128" (zig 0.8.0)
fn exp(x: f128) f128 {
    if (x > 1) {
        const y = exp(x / 2);
        return y * y;
    }

    if (x < 0) {
        return 1 / exp(-x);
    }

    const n = 16384;
    const dx = x / n;
    var y: f128 = 1;
    var i: u32 = 0;

    while (i < n) : (i += 1) {
        y *= (2 + dx) / (2 - dx);
    }

    return y;
}
