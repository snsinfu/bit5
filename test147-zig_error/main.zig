const std = @import("std");

pub fn main() void {
    run() catch |err| {
        std.debug.print("error: {}\n", .{err});
    };
}

fn run() !void {
    const config = loadConfig("app.conf") catch |err| {
        return err;
    };

    var i: i32 = 0;
    while (i < config.count) : (i += 1) {
        std.debug.print("{}\n", .{i});
    }
}

const ConfigError = error{
    Input,
    BadSyntax,
    BadValue,
};

const Config = struct {
    count: i32,
    filename: []const u8,
};

fn loadConfig(filename: []const u8) ConfigError!Config {
    if (filename.len == 0) {
        return ConfigError.Input;
    }
    return Config{
        .count = 10,
        .filename = filename,
    };
}
