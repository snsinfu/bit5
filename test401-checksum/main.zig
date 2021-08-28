const std = @import("std");
const print = std.debug.print;

// IPv4 header checksumming
// https://everything2.com/index.pl?node_id=441121

pub fn main() void {
    var data = [_]u16{
        //      vvvvvv checksum field
        0x6547, 0x0000, 0xcad6, 0x28f2, 0xfab7, 0x0a76, 0x634b, 0xbd13,
        0x26be, 0x728d, 0xda8e, 0x5413, 0x0b54, 0x4868, 0x5739, 0x9633,
    };

    // Fill in the checksum field of the data.
    data[1] = checksum(&data);

    // Calculating the checksum of the data must give zero.
    const sum = checksum(&data);

    print("Checksum:     {x:0>4}\n", .{data[1]});
    print("Verification: {x:0>4}\n", .{sum});
}

fn checksum(data: []const u16) u16 {
    var sum: u32 = 0;
    for (data) |x| {
        sum += x;
    }

    while (sum > 0xffff) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~@intCast(u16, sum);
}
