const print = @import("std").debug.print;

pub fn main() void {
    // Simulating SIPO shift registers.
    // https://en.wikipedia.org/wiki/Shift_register

    // We parallelize this serial input.
    const serial_input = [_]u1{0, 1, 1, 0, 0, 1, 0, 1};

    // u8 as an array of flip flop (FF) modules. The lowest-bit FF is connected
    // to the input. The output of each FF is connected to the input of the FF
    // at one bit higher position.
    //
    // input -> [0] -> [1] -> [2] -> [3] -> [4] -> [5] -> [6] -> [7]
    //           |      |      |      |      |      |      |      |
    //         clock  clock  clock  clock  clock  clock  clock  clock
    //
    var sreg: u8 = 0;

    for (serial_input) |input| {
        // Clock. The output of each FF is transfered to the next FF.
        sreg <<= 1;

        // The first FF receives the input bit.
        sreg |= input;

        // Observe parallel output.
        print("{b:0>8}\n", .{sreg});
    }
}
