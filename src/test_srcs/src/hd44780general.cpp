#ifndef HD44780GENERAL_HPP
#include "hd44780general.hpp"
#endif 
#include <sstream>
#include <bitset>
#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

const int HD44780_LINE_START_LOC[] = {
    HD44780_START_ADD_L1,
    HD44780_START_ADD_L2,
    HD44780_START_ADD_L3,
    HD44780_START_ADD_L4
};

// Pins assigned for the HD44780 interface
// Only 4 high used if set to 4 bit mode at HD44780_MODE
#if HD44780_CONFIG_DL_DATA_LENGTH == 1
#define HD44780_MODE 8
#elif HD44780_CONFIG_DL_DATA_LENGTH == 0
#define HD44780_MODE 4
#else
#error INVALID HD44780_CONFIG_DL_DATA_LENGTH MUST BE EITHER 0 or 1
#endif
const int HD44780_PIN_COUNT = HD44780_MODE;

#define NROW 4
#define ROWLEN 17
char hd44780_display_data[NROW][ROWLEN] = {
    "",
    "",
    "",
    "",
};

int get_high_4bits(const int v) {
    return (v & 0xF0) >> 4;
}

int get_low_4bits(const int v) {
    return v & 0x0F;
}

int valPos(int pos, int vals) {
    // Force to be 1 or 0
    return (vals & (1u << pos)) ? 1 : 0;
}
/* 
 * All operations are:
 * - HD44780_PINS_E goes high
 * - HD44780_PINS_DATA gets set
 * - Wait until the 1/500k for the clock to elapse the time
 * - HD44780_PINS_E goes low, committing the instruction
 */
#if HD44780_CONFIG_DL_DATA_LENGTH == 1
class hd44780payload {
public:
    hd44780payload(int payload, bool rss) { bits = payload; rs = rss;}
    int getbits(){ return bits; }
    int getrs(){ return rs; }
private:
    bool rs;
    int bits;
};

#elif HD44780_CONFIG_DL_DATA_LENGTH == 0
class hd44780payload {
public:
    hd44780payload(int payload, bool rss) {
        onlyhigh = false;
        rs = rss;
        highbits = get_high_4bits(payload);
        lowbits = get_low_4bits(payload);
    }
    hd44780payload(int payload, bool rss, bool oh) {
        onlyhigh = oh;
        rs = rss;
        highbits = get_high_4bits(payload);
        lowbits = get_low_4bits(payload);
    }
    int getrs(){ return rs; }
    int gethighbits(){ return highbits; }
    int getlowbits(){ return lowbits; }
    std::string to_string() {
        std::stringstream ss;
        ss << "HighBits: " << std::bitset<4>((unsigned int)highbits);
        ss << " LowBits: " << std::bitset<4>((unsigned int)lowbits);
        ss << " RS: " << (unsigned int)rs;
        ss << " OnlyH: " << (unsigned int)onlyhigh;
        return ss.str();
    }

private:
    bool rs;
    bool onlyhigh;
    int highbits;
    int lowbits;
};
#endif

hd44780payload hd44780_send_payload(const int v, bool rs) {
    return hd44780payload(v, rs);
}

hd44780payload hd44780_send_instruction(const int v) {
    //gpio_put( HD44780_PINS_RS, 0 );
    return hd44780_send_payload(v, false);
}

hd44780payload hd44780_send_data_payload(const int v) {
    //gpio_put( HD44780_PINS_RS, 1 );
    return hd44780_send_payload(v, true);
}

hd44780payload hd44780_inst_display_clear() {
    // Per instructions:
    // - DB7: 0
    // - DB6: 0
    // - DB5: 0
    // - DB4: 0
    // - DB3: 0
    // - DB2: 0
    // - DB1: 0
    // - DB0: 1
    const int val = 0x01;
    return hd44780_send_instruction(val);
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
}

hd44780payload hd44780_inst_return_home() {
    // Per instructions:
    // - DB7: 0
    // - DB6: 0
    // - DB5: 0
    // - DB4: 0
    // - DB3: 0
    // - DB2: 0
    // - DB1: 1
    // - DB0: Ignored
    const int val = 0x02;
    return hd44780_send_instruction(val);
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
}

#define HD44780_CONFIG_ID_INCREMENT_DIRECTION 1 // 1 - Right | 0 - Left
#define HD44780_CONFIG_SHIFT_CURSOR 1 // 0 off | 1 on 
hd44780payload hd44780_inst_entry_mode_set(const int id, const int s) {
    // Per instructions:
    // - DB7: 0
    // - DB6: 0
    // - DB5: 0
    // - DB4: 0
    // - DB3: 0
    // - DB2: 1
    // - DB1: ID // Cursor direction // 1 - Right | 0 - Left
    // - DB0: S  // Shift after read/write // 0 off | 1 on
    const int val = 0x04 |
        (id & 0x01) << 1 |
        (s & 0x01)
        ;
    return hd44780_send_instruction(val);
}

hd44780payload hd44780_inst_display_control(const int d, const int c, const int b) {
    // Per instructions:
    // - DB7: 0
    // - DB6: 0
    // - DB5: 0
    // - DB4: 0
    // - DB3: 1
    // - DB2: D // Display on/off
    // - DB1: C // Cursor on/off
    // - DB0: B // Blink cursor on/off
    const int val = 0x08 |
        (d &0x01) << 2 |
        (c &0x01) << 1 |
        (b &0x01) 
        ;
    return hd44780_send_instruction(val);
}

hd44780payload hd44780_inst_cursor_display_shift(const int sc, const int rl) {
    // Per instructions:
    // - DB7: 0
    // - DB6: 0
    // - DB5: 0
    // - DB4: 1
    // - DB3: SC
    // - DB2: RL
    // - DB1: Ignored 
    // - DB0: Ignored
    // SC - RL Table:
    //  0    0 Shift cursor to the left (AC-1)
    //  0    1 Shift cursor to the right (AC+1)
    //  1    0 Shift display to the left, cursor follows
    //  1    1 Shift display to the right, cursor follows

    // Start with the instruction value
    const int val = 0x10 |
        sc << 3 | 
        rl << 2
        ;
    return hd44780_send_instruction(val);
}

// This specific call is to transition from 8 bit to 4 bit mode
hd44780payload hd44780_inst_function_set_half() {
    // Per instructions:
    // - DB7: 0
    // - DB6: 0
    // - DB5: 1
    // - DB4: DL
    // - DB3: N
    // - DB2: F
    // - DB1: Ignored
    // - DB0: Ignored

    // Start with the instruction value
    const int val = 0x20 |
        HD44780_CONFIG_DL_DATA_LENGTH << 4 | 
        HD44780_CONFIG_N_DISPLAY_LINES << 3 |
        HD44780_CONFIG_F_CHARACTER_FONT << 2
        ;
    return hd44780payload(val, true);
}

hd44780payload hd44780_inst_function_set() {
    // Per instructions:
    // - DB7: 0
    // - DB6: 0
    // - DB5: 1
    // - DB4: DL
    // - DB3: N
    // - DB2: F
    // - DB1: Ignored
    // - DB0: Ignored

    // Start with the instruction value
    const int val = 0x20 |
        HD44780_CONFIG_DL_DATA_LENGTH << 4 | 
        HD44780_CONFIG_N_DISPLAY_LINES << 3 |
        HD44780_CONFIG_F_CHARACTER_FONT << 2
        ;
    return hd44780_send_instruction(val);
}

hd44780payload hd44780_inst_set_cgram_address(const int address) {
    // Per instructions:
    // - DB7: 0
    // - DB6: 1
    // - DB5: Add
    // - DB4: Add
    // - DB3: Add
    // - DB2: Add
    // - DB1: Add
    // - DB0: Add

    // Start with the instruction value
    const int val = 0x40 |
        (address & 0x3F)
        ;
    return hd44780_send_instruction(val);
}

hd44780payload hd44780_inst_set_ddram_address(const int address) {
    // Per instructions:
    // - DB7: 1
    // - DB6: Add
    // - DB5: Add
    // - DB4: Add
    // - DB3: Add
    // - DB2: Add
    // - DB1: Add
    // - DB0: Add

    // Start with the instruction value
    const int val = 0x80 |
        (address & 0x7F)
        ;
    return hd44780_send_instruction(val);
}

void reset_sequence() {
    // Fully wait until initialization is compleated
    //vTaskDelayUntil( , hd44780_POWERON_DELAY_MS );
    // Instruction to archieve the correct initialization
#if HD44780_CONFIG_DL_DATA_LENGTH == 0
    hd44780_inst_function_set_half();
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
#endif
    hd44780_inst_function_set();
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
    hd44780_inst_display_clear();
    hd44780_inst_display_control(1, 1, 0);
    hd44780_inst_entry_mode_set(1,0);

    // Set start point
    hd44780_inst_set_ddram_address(HD44780_START_ADD_L4);
}
