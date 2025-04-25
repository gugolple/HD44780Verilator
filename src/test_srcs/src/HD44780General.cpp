#ifndef HD44780GENERAL_HPP
#include "HD44780General.hpp"
#endif 
#include <bitset>
#include <iostream>

const int HD44780_LINE_START_LOC[] = {
    HD44780_START_ADD_L1,
    HD44780_START_ADD_L2,
    HD44780_START_ADD_L3,
    HD44780_START_ADD_L4
};

const int HD44780_PIN_COUNT = HD44780_MODE;

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

HD44780Payload hd44780_send_payload(const int v, bool rs, bool oh) {
    return HD44780Payload(v, rs, oh);
}

HD44780Payload hd44780_send_payload(const int v, bool rs) {
    return hd44780_send_payload(v, rs, false);
}

HD44780Payload hd44780_send_instruction(const int v) {
    return hd44780_send_payload(v, false);
}

HD44780Payload hd44780_send_instruction_onlyhigh(const int v) {
    return hd44780_send_payload(v, false, true);
}

HD44780Payload hd44780_send_data_payload(const int v) {
    return hd44780_send_payload(v, true);
}

HD44780Payload hd44780_inst_display_clear() {
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

HD44780Payload hd44780_inst_return_home() {
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

HD44780Payload hd44780_inst_entry_mode_set(const int id, const int s) {
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

HD44780Payload hd44780_inst_display_control(const int d, const int c,
        const int b) {
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

HD44780Payload hd44780_inst_cursor_display_shift(const int sc, const int rl) {
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
HD44780Payload hd44780_inst_function_set_half() {
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
    return hd44780_send_instruction_onlyhigh(val);
}

HD44780Payload hd44780_inst_function_set() {
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

HD44780Payload hd44780_inst_set_cgram_address(const int address) {
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

HD44780Payload hd44780_inst_set_ddram_address(const int address) {
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

#if HD44780_CONFIG_DL_DATA_LENGTH == 1
HD44780Payload::HD44780Payload(int payload, bool rss) { 
    bits = payload;
    rs = rss;
}
int HD44780Payload::getbits(){
    return bits;
}
int HD44780Payload::getrs(){
    return rs;
}
#elif HD44780_CONFIG_DL_DATA_LENGTH == 0
HD44780Payload::HD44780Payload(int payload, bool rss) {
    onlyhigh = false;
    rs = rss;
    highbits = get_high_4bits(payload);
    lowbits = get_low_4bits(payload);
}

HD44780Payload::HD44780Payload(int payload, bool rss, bool oh) {
    onlyhigh = oh;
    rs = rss;
    highbits = get_high_4bits(payload);
    lowbits = get_low_4bits(payload);
}

int HD44780Payload::getrs() const { 
    return rs;
}

int HD44780Payload::gethighbits() const { 
    return highbits; 
}

int HD44780Payload::getlowbits() const { 
    return lowbits;
}

std::string HD44780Payload::to_string() const {
    std::stringstream ss;
    ss << "HighBits: " << std::bitset<4>((unsigned int)highbits);
    ss << " LowBits: " << std::bitset<4>((unsigned int)lowbits);
    ss << " RS: " << (unsigned int)rs;
    ss << " OnlyH: " << (unsigned int)onlyhigh;
    return ss.str();
}

void reset_sequence() {
    // Fully wait until initialization is compleated
    //vTaskDelayUntil( , hd44780_POWERON_DELAY_MS );
    // Instruction to archieve the correct initialization
#if HD44780_CONFIG_DL_DATA_LENGTH == 0
    std::cout << hd44780_inst_function_set_half().to_string() << std::endl;
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
#endif
    std::cout << hd44780_inst_function_set().to_string() << std::endl;
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
    std::cout << hd44780_inst_display_clear().to_string() << std::endl;
    std::cout << hd44780_inst_display_control(
            HD44780_CONFIG_D_DISPLAY_ONOFF,
            HD44780_CONFIG_C_CURSOR_ONOFF,
            HD44780_CONFIG_C_CURSOR_BLINK
            ).to_string() 
        << std::endl;
    std::cout << hd44780_inst_entry_mode_set(
            HD44780_CONFIG_ID_INCREMENT_DIRECTION,
            HD44780_CONFIG_SHIFT_CURSOR
            ).to_string()
        << std::endl;

    // Set start point
    std::cout << "Print all starting points" << std::endl;
    std::cout << hd44780_inst_set_ddram_address(HD44780_START_ADD_L1).to_string() 
        << std::endl;
    std::cout << hd44780_inst_set_ddram_address(HD44780_START_ADD_L2).to_string() 
        << std::endl;
    std::cout << hd44780_inst_set_ddram_address(HD44780_START_ADD_L3).to_string() 
        << std::endl;
    std::cout << hd44780_inst_set_ddram_address(HD44780_START_ADD_L4).to_string() 
        << std::endl;
}
#endif
