// kernel.cpp - MiniOS core (fixed version)

#include <stdint.h>

// =========================
// BASIC TYPES (freestanding)
// =========================
typedef unsigned char bool;
#define true 1
#define false 0

extern "C" void kmain();

// =========================
// VGA TEXT MODE
// =========================
volatile uint16_t* VGA = (uint16_t*)0xB8000;

static int cursor_x = 0;
static int cursor_y = 0;

static inline void vga_put(char c, uint8_t color = 0x07) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }

    VGA[cursor_y * 80 + cursor_x] = (uint16_t)c | (uint16_t)color << 8;

    cursor_x++;
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= 25) {
        // scroll
        for (int y = 1; y < 25; y++) {
            for (int x = 0; x < 80; x++) {
                VGA[(y - 1) * 80 + x] = VGA[y * 80 + x];
            }
        }
        cursor_y = 24;
    }
}

static void print(const char* s) {
    while (*s) vga_put(*s++);
}

// =========================
// I/O PORTS
// =========================
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
}

// =========================
// KEYBOARD (polling PS/2)
// =========================
static const char scancode_map[128] = {
    0, 27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    ' ','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',0,
    '\\','z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
};

static const char shift_map[128] = {
    0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
    ' ','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',0,
    '|','Z','X','C','V','B','N','M','<','>','?',0,'*',0,' '
};

static char read_key() {
    static bool shift = false;

    if (!(inb(0x64) & 1))
        return 0;

    uint8_t sc = inb(0x60);

    if (sc == 0x2A || sc == 0x36) { shift = true; return 0; }
    if (sc == 0xAA || sc == 0xB6) { shift = false; return 0; }
    if (sc & 0x80) return 0;

    return shift ? shift_map[sc] : scancode_map[sc];
}

static int readline(char* buf, int max) {
    int pos = 0;

    while (1) {
        char c = read_key();
        if (!c) continue;

        if (c == '\n') {
            vga_put('\n');
            buf[pos] = 0;
            return pos;
        }

        if (c == '\b') {
            if (pos > 0) {
                pos--;
                vga_put('\b');
            }
            continue;
        }

        if (pos < max - 1) {
            buf[pos++] = c;
            vga_put(c);
        }
    }
}

// =========================
// SIMPLE CALCULATOR
// =========================
static const char* expr;

static void skip() { while (*expr == ' ') expr++; }

static long parse_number() {
    skip();

    int sign = 1;
    if (*expr == '-') { sign = -1; expr++; }

    long val = 0;
    while (*expr >= '0' && *expr <= '9') {
        val = val * 10 + (*expr - '0');
        expr++;
    }

    return val * sign;
}

static long parse_expr();

static long parse_term() {
    long v = parse_number();

    while (1) {
        skip();
        if (*expr == '*') { expr++; v *= parse_number(); }
        else if (*expr == '/') { expr++; long d = parse_number(); if (d) v /= d; }
        else break;
    }

    return v;
}

static long parse_expr() {
    long v = parse_term();

    while (1) {
        skip();
        if (*expr == '+') { expr++; v += parse_term(); }
        else if (*expr == '-') { expr++; v -= parse_term(); }
        else break;
    }

    return v;
}

static long eval(const char* s) {
    expr = s;
    return parse_expr();
}

// =========================
// MAIN KERNEL
// =========================
extern "C" void kmain() {

    // clear screen
    for (int y = 0; y < 25; y++)
        for (int x = 0; x < 80; x++)
            VGA[y * 80 + x] = ' ' | (0x07 << 8);

    print("MiniOS Kernel OK\n");
    print("Calculator ready\n");
    print("Example: 12+34\n\n");

    char buf[128];

    while (1) {
        print("> ");
        readline(buf, 128);

        long r = eval(buf);

        char out[32];
        int i = 0;

        if (r == 0) {
            out[i++] = '0';
        } else {
            if (r < 0) {
                out[i++] = '-';
                r = -r;
            }

            char tmp[32];
            int t = 0;

            while (r > 0) {
                tmp[t++] = '0' + (r % 10);
                r /= 10;
            }

            while (t--) out[i++] = tmp[t];
        }

        out[i] = 0;

        print("= ");
        print(out);
        print("\n");
    }
}
