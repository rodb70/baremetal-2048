#ifndef INPUT_H
#define INPUT_H

uint16_t key_cur;
uint16_t key_prev;

#define REG_KEYINPUT  (* (volatile uint32_t*) 0x4000130)

#define KEY_A        0x0001
#define KEY_B        0x0002
#define KEY_SELECT   0x0004
#define KEY_START    0x0008
#define KEY_RIGHT    0x0010
#define KEY_LEFT     0x0020
#define KEY_UP       0x0040
#define KEY_DOWN     0x0080
#define KEY_R        0x0100
#define KEY_L        0x0200

#define KEY_MASK     0x03FF

// Polling function
static inline void key_poll(void)
{
    key_prev = key_cur;
    key_cur = ~REG_KEYINPUT & KEY_MASK;
}

// Basic state checks
static inline uint32_t key_curr_state(void)
{
    return key_cur;
}

static inline uint32_t key_prev_state(void)
{
    return key_prev;
}

static inline uint32_t key_is_down(uint32_t key)
{
    return key_cur & key;
}

static inline uint32_t key_is_up(uint32_t key)
{
    return ~key_cur & key;
}

static inline uint32_t key_was_down(uint32_t key)
{
    return key_cur & key;
}

static inline uint32_t key_was_up(uint32_t key)
{
    return ~key_cur & key;
}

#endif
