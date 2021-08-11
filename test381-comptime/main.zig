fn BList(comptime T: type, comptime N: usize) type {
    return struct {
        next: ?*@This() = null,
        values: [N]T = undefined,
    };
}

test "BList" {
    var b = BList(i32, 4){};
    var a = BList(i32, 4){.next = &b};

    a.values = [_]i32{1, 2, 3, 4};
    b.values = [_]i32{5, 6, 7, 8};

    var p: ?*BList(i32, 4) = &a;
    while (p != null) {
        p = p.?.next;
    }
}
