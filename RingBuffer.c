#include <stdio.h>
#include <stdint.h>

#define SIZE 8

uint8_t buf[SIZE];
int head  = 0;
int tail  = 0;
int cnt   = 0;

int write_byte(uint8_t data)
{
    if (cnt == SIZE)
        return -1;
    buf[head] = data;
    head = (head + 1) % SIZE;
    cnt++;
    return 0;
}

int read_byte(uint8_t *out)
{
    if (cnt == 0)
        return -1;
    *out = buf[tail];
    tail = (tail + 1) % SIZE;
    cnt--;
    return 0;
}

int main(void)
{
    uint8_t val;
    int i, r;

    uint8_t d1[] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};
    for (i = 0; i < 8; i++) {
        r = write_byte(d1[i]);
        if (r == 0) {
            if (cnt == SIZE)
                printf("[WRITE] 0x%02X -> OK (count=%d) FULL\n", d1[i], cnt);
            else
                printf("[WRITE] 0x%02X -> OK (count=%d)\n", d1[i], cnt);
        }
    }

    r = write_byte(0x99);
    if (r == -1)
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");

    for (i = 0; i < 3; i++) {
        r = read_byte(&val);
        if (r == 0)
            printf("[READ]  -> 0x%02X (count=%d)\n", val, cnt);
    }

    uint8_t d2[] = {0x49, 0x4A, 0x4B};
    for (i = 0; i < 3; i++) {
        r = write_byte(d2[i]);
        if (r == 0) {
            if (cnt == SIZE)
                printf("[WRITE] 0x%02X -> OK (count=%d) FULL\n", d2[i], cnt);
            else
                printf("[WRITE] 0x%02X -> OK (count=%d)\n", d2[i], cnt);
        }
    }

    while (cnt > 0) {
        r = read_byte(&val);
        if (r == 0) {
            if (cnt == 0)
                printf("[READ]  -> 0x%02X (count=%d) EMPTY\n", val, cnt);
            else
                printf("[READ]  -> 0x%02X (count=%d)\n", val, cnt);
        }
    }

    r = read_byte(&val);
    if (r == -1)
        printf("[READ]  (empty) -> FAIL (buffer empty)\n");

    return 0;
}