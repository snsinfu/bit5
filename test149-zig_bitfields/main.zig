const std = @import("std");

pub fn main() void {
    const value: f32 = -1.23e4;
    const repr = @bitCast(Single, value);
    std.debug.print("{}\n", .{repr});
}

const Single = packed struct{
    // We expect a little-endian CPU here.
    mantissa: u23,
    exponent: u8,
    sign: u1,
};

const expect = std.testing.expect;

test "Single is 32-bit" {
    expect(@sizeOf(Single) == 4);

    // -1.5
    {
        const repr = @bitCast(Single, @floatCast(f32, -1.5));
        expect(repr.mantissa == 0x400000);
        expect(repr.exponent == 127);
        expect(repr.sign == 1);
    }
}
