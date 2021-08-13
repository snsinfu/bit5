const std = @import("std");
const print = std.debug.print;

// 128-bit bloom filter-like hash set. I hacked up a bad hash function for this,
// and it worked surprisingly well...

pub fn main() void {
    const keys = [_][]const u8{ "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog", "lorem", "ipsum", "dolor", "sit", "amet", "etaoin", "shrdlu", "Portez", "ce", "vieux", "whisky", "au", "juge", "blond", "qui", "fume" };
    var set: u128 = 0;

    for (keys) |key, i| {
        if (@mod(i, 2) == 0) {
            set |= weakHash(key);
            print("inserted '{s}'\n", .{key});
        }
    }

    for (keys) |key| {
        const h = weakHash(key);
        if (set & h == h) {
            print("set may contain '{s}'\n", .{key});
        }
    }
}

fn weakHash(str: []const u8) u128 {
    const h = hash(str);
    const x = (h >> 118) | (h << 10);
    const y = (h >> 100) | (h << 28);
    const z = (h >> 51) | (h << 77);
    const w = (h >> 17) | (h << 111);
    return x & y & z & w;
}

fn hash(str: []const u8) u128 {
    var val: u128 = 0;
    var cc: u8 = 0;

    for (str) |c| {
        cc +%= c;
        const n = @truncate(u7, (cc >> 2) + (cc & 3));
        const x = val >> n;
        const y = val << (127 - n);
        const c1 = @as(u128, c) * 1234567890_1234567890_1234567890;
        const c2 = @as(u128, cc) * 2345678901_2345678901_2345678901;
        const c3 = @as(u128, c) * 3456789012_3456789012_3456789012;
        const c4 = @as(u128, cc) * 4567890123_4567890123_4567890123;
        val = (x | y) ^ (c1 + c2 + c3 + c4);
    }

    return val;
}
