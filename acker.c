#include "common.h"

// 006049 JT R0 6057
// 006052 ADD R0 R1 1
// 006056 RET
// 006057 JT R1 6070
// 006060 ADD R0 R0 32767
// 006064 SET R1 R7
// 006067 CALL 6049
// 006069 RET
// 006070 PUSH R0
// 006072 ADD R1 R1 32767
// 006076 CALL 6049
// 006078 SET R1 R0
// 006081 POP R0
// 006083 ADD R0 R0 32767
// 006087 CALL 6049
// 006089 RET

typedef uint16_t u16;

#if 0
// u16 r0, r1, r7;

void function()
{
    if (!r0) {
        r0 = (r1 + 1) % 32768;
        return;
    }

    if (!r1) {
        r0 = (r0 + 32767) % 32768;
        r1 = r7;
        function();
        return;
    }

    u16 a = r0;
    r1 = (r1 + 32767) % 32768;
	function();
    r1 = r0;
    r0 = a;
    r0 = (r0 + 32767) % 32768;
    function();
}

u16 function3(u16 r0, u16 r1, u16 r7)
{
    if (!r0) {
        return r1 + 1;
    }

    if (!r1) {
        return function3(r0 - 1, r7, r7);
    }

    return function3(r0 - 1, function3(r0, r1 - 1, r7) - 1, r7);
}
#endif

typedef struct Acker {
    u32 key;
    u16 value;
} Acker;

Acker *acker = NULL;

u32 keymake(u16 r0, u16 r1)
{
    u32 key = 0;
    key = ((u32)r0) << 16;
    key |= r1;
    return key;
}

i32 lookup(u32 key, u16 *out)
{
    if (hmgeti(acker, key) >= 0) {
        *out = hmget(acker, key);
        return true;
    } else {
        return false;
    }
}

u16 store(u32 key, u16 value)
{
    hmput(acker, key, value);
    return value;
}

u16 function2(u16 r0, u16 r1, u16 r7)
{
    u32 key = keymake(r0, r1);

    u16 out = 0;

    if (lookup(key, &out)) {
        return out;
    }

    if (!r0) {
        u16 ans = (r1 + 1) % 32768;
        return store(key, ans);
    }

    if (!r1) {
        r0 = (r0 + 32767) % 32768;
        u16 ans = function2(r0, r7, r7);
        return store(key, ans);
    }

    u16 a = r0;
    r1 = (r1 + 32767) % 32768;
	r1 = function2(r0, r1, r7);
    r0 = (a + 32767) % 32768;
    u16 ans = function2(r0, r1, r7);
    return store(key, ans);
}

int main()
{
    u16 r0 = 4;
    u16 r1 = 1;
    u16 ans = 0;

    for (i32 i = 1; i < 32768 && ans != 6; i++) {
        printf("trying %d ", i);
        ans = function2(r0, r1, (u16)i);
        printf("ans = %hu\n", ans);
        hmfree(acker);
        acker = NULL;
    }
}
