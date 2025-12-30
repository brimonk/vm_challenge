#include "common.h"

enum {
    DIRECTION_N,
    DIRECTION_E,
    DIRECTION_S,
    DIRECTION_W,
    DIRECTION_TOTAL
};

typedef struct Room {
    i32 rooms[DIRECTION_TOTAL];
    enum {
        ROOM_SYMBOL_NUMBER,
        ROOM_SYMBOL_OPERATOR
    } kind;
    i32 value;
} Room;

Room ROOMS[4][4] = {
    {
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '*' },
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 8   },
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '-' },
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 1   },
    },
    {
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 4   },
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '*' },
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 11  },
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '*' },
    },
    {
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '+' },
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 4   },
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '-' },
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 18  },
    },
    {
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 22  },
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '-' },
        { .kind = ROOM_SYMBOL_NUMBER,   .value = 9   },
        { .kind = ROOM_SYMBOL_OPERATOR, .value = '-' },
    }
};

typedef struct Orb {
    Room *room;
    i32 value;
} Orb;

#define MAX_DEPTH 14

i32 IsTraversalSolved(Orb **path, i32 cost_target, i32 dx, i32 dy)
{
    Orb last = arrlast(*path);
    return last.value == cost_target && last.room == &ROOMS[dy][dx];
}

void RoomTraversal(Orb **path, i32 cost_target, i32 x, i32 y, i32 dx, i32 dy)
{
    if (x < 0 || x > 3 || y < 0 || y > 3) { // OOB
        return;
    }

    if (arrlen(*path) > 1 && (x == 0 && y == 3)) { // We cannot revisit the starting spot
        return;
    }

    Orb last = arrlast(*path);

    printf("Visiting %d, %d\n", x, y);
    // We stop traversing if we've solved the traversal, whether or not we have a real value or not
    if (IsTraversalSolved(path, cost_target, dx, dy) || last.room == &ROOMS[dy][dx]) {
        return; // we actually have a path, and we're done
    }

    if (arrlen(*path) >= MAX_DEPTH) {
        return;
    }

    Orb curr = { .room = &ROOMS[y][x] };

    if (last.room->kind == ROOM_SYMBOL_NUMBER) {
        curr.value = last.value;
    } else {
        switch (last.room->value) {
        case '+':
            curr.value = last.value + curr.room->value;
            break;
        case '*':
            curr.value = last.value * curr.room->value;
            break;
        case '-':
            curr.value = last.value - curr.room->value;
            break;
        default:
            assert(0);
             break;
        }
    }

    arrput(*path, curr);
    i32 pretraverse_len = arrlen(*path);

    RoomTraversal(path, cost_target, x + 1, y + 0, dx, dy);
    if (!IsTraversalSolved(path, cost_target, dx, dy)) {
        while (pretraverse_len < arrlen(*path)) {
            Orb remove = arrpop(*path);
            (void)remove;
        }
    }

    RoomTraversal(path, cost_target, x - 1, y + 0, dx, dy);
    if (!IsTraversalSolved(path, cost_target, dx, dy)) {
        while (pretraverse_len < arrlen(*path)) {
            Orb remove = arrpop(*path);
            (void)remove;
        }
    }

    RoomTraversal(path, cost_target, x + 0, y + 1, dx, dy);
    if (!IsTraversalSolved(path, cost_target, dx, dy)) {
        while (pretraverse_len < arrlen(*path)) {
            Orb remove = arrpop(*path);
            (void)remove;
        }
    }

    RoomTraversal(path, cost_target, x + 0, y - 1, dx, dy);
    if (!IsTraversalSolved(path, cost_target, dx, dy)) {
        while (pretraverse_len < arrlen(*path)) {
            Orb remove = arrpop(*path);
            (void)remove;
        }
    }
}

int main()
{
    assert(ARRSIZE(ROOMS) == 4);
    assert(ARRSIZE(ROOMS[0]) == 4);

    autofreearr Orb *orb = NULL;
    Orb start = { .room = &ROOMS[3][0], .value = 22 };
    arrput(orb, start);

    RoomTraversal(&orb, 30, 0, 3, 3, 0);

    i32 i = 0;
    do {
        if (i > 0)
            printf("-> ");
        printf("%d : ", orb[i].value);
        if (orb[i].room->kind == ROOM_SYMBOL_NUMBER) {
            printf("%d\n", orb[i].room->value);
        } else {
            printf("%c\n", (char)orb[i].room->value);
        }
    } while (++i < arrlen(orb));

    for (i32 i = 0; i < arrlen(orb); i++) {
    }
}
