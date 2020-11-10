const std = @import("std");

pub fn main() void {
    // Exotic bit size
    var n: u15 = 32767;
    var m: u3 = @intCast(u3, n & 0b111);

    // Wide integer (would be useful for crypto and PRNG stuffs)
    // XXX: Compile fails or crashes if you use u129 or larger (on x86-64?)
    var a: u128 = 1;
    a <<= 100;

    std.debug.print("{}\n", .{m});
    std.debug.print("{}\n", .{a});
}
