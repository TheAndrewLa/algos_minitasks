const std = @import("std");

const debug = std.debug;
const log = std.log;
const memory = std.mem;

// Delimiters
const crlf: comptime_int = '\r';
const lf: comptime_int = '\n';
const space: comptime_int = ' ';

// Brackets
const circle_bracket_open: comptime_int = '(';
const circle_bracket_close: comptime_int = ')';

const box_bracket_open: comptime_int = '[';
const box_bracket_close: comptime_int = ']';

// NOTE: just for good syntax btw
// TODO: create exception class and include alloc error in exceptions
const alloc_error = memory.Allocator.Error;

threadlocal var str_allocator: ?memory.Allocator = null;
threadlocal var tokens_allocator: ?memory.Allocator = null;

pub const token = struct {
    pub const types = enum {
        operator,

        // NOTE: it's better to keep it as operand
        number,

        opening_barcket,
        closing_bracket,
    };

    // NOTE: determination of type is not a part of lexical analysis, so it will be filled later
    what: ?types,

    content: [*]u8,
    length: usize,

    pub fn new(raw: []const u8) alloc_error!token {
        debug.assert(str_allocator != null);

        var ptr = try str_allocator.alloc(u8, raw.len);
        memory.copy(u8, new, raw);

        return token{
            null,
            ptr,
            raw.len,
        };
    }

    pub fn delete(self: token) void {
        debug.assert(str_allocator != null);
        str_allocator.free(self.content);
    }
};

pub const token_array = struct {
    array: std.ArrayList(token),

    pub fn new(expr: []const u8) alloc_error!token_array {}

    pub fn delete(self: token_array) void {}
};

pub fn parse_expression() anyerror!void {
    log.err("Error!", .{});
}
