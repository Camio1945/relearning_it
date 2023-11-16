const std = @import("std");
pub fn main() !void {
    const start: i64 = std.time.milliTimestamp();
    const dict = @embedFile("Dictionary.txt");
    var iterator = std.mem.split(u8, dict, "\r\n");
    var list = std.ArrayList([]const u8).init(std.heap.page_allocator);
    defer list.deinit();
    while (iterator.next()) |line| {
        var is_word: bool = true;
        var score_sum: i32 = 0;
        for (line) |c| {
            if (c < 'a' or c > 'z') {
                is_word = false;
                break;
            } else {
                score_sum += (c - 'a') + 1;
            }
        }
        if (is_word and score_sum == 100) {
            try list.append(line);
        }
    }
    std.debug.print("{s}\n\n", .{list.items});
    std.debug.print("words number: {d}\n", .{list.items.len});
    std.debug.print("        time: {d} ms\n", .{std.time.milliTimestamp() - start});
}
