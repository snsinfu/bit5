const print = @import("std").debug.print;

pub fn main() void {
    var num: u128 = 1234567890_1234567890_1234567890;
    var count: u32 = 0;

    while (num > 1) {
        print("{}\t{}\n", .{count, num});

        if (@rem(num, 2) == 0) {
            num = @divTrunc(num, 2);
        } else {
            num = num * 3 + 1;
        }

        count += 1;
    }

    print("{}\tdone.\n", .{count});
}
