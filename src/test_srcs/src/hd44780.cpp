#include "hd44780.hpp"
#include <sstream>
#include <bitset>

#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

// Hardware configuration of unit
#define HD44780_CONFIG_DL_DATA_LENGTH   0 //0 - 4 | 1 - 8 //Bits for comm
#define HD44780_CONFIG_N_DISPLAY_LINES  1 //0 - 1 | 1 - 2
#define HD44780_CONFIG_F_CHARACTER_FONT 0 

// Macro definition checks
// HD44780_CONFIG_N_DISPLAY_LINES
#if HD44780_CONFIG_N_DISPLAY_LINES != 0 && HD44780_CONFIG_N_DISPLAY_LINES != 1
#error
#error INVALID HD44780_CONFIG_N_DISPLAY_LINES MUST BE EITHER 0 or 1
#endif

// HD44780_CONFIG_F_CHARACTER_FONT
#if HD44780_CONFIG_F_CHARACTER_FONT != 0 && HD44780_CONFIG_F_CHARACTER_FONT != 1
#error
#error INVALID HD44780_CONFIG_F_CHARACTER_FONT MUST BE EITHER 0 or 1
#endif

// Hardware restrictions of official spec
#define MAX_HD44780_FREQ              ( 250000 ) //Herth
#define MIN_HD44780_PERIOD_US         ( 1000000/MAX_HD44780_FREQ )
#define hd44780_POWERON_DELAY_MS      ( 100 / portTICK_PERIOD_MS )
#define hd44780_INST_CLEAR_DISPLAY_MS ( 10 / portTICK_PERIOD_MS )
#define hd44780_INST_DELAY_US         80

#define HD44780_START_ADD_L1          (0x00) 
#define HD44780_START_ADD_L2          (0x40)
#define HD44780_START_ADD_L3          (0x10) 
#define HD44780_START_ADD_L4          (0x50)
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
const int HD44780_PINS_DATA[HD44780_MODE] = {1,2,3,4,5,6,7,8};
#elif HD44780_CONFIG_DL_DATA_LENGTH == 0
#define HD44780_MODE 4
const int HD44780_PINS_DATA[HD44780_MODE] = {5,6,7,8};
#else
#error INVALID HD44780_CONFIG_DL_DATA_LENGTH MUST BE EITHER 0 or 1
#endif
const int HD44780_PINS_RW   = 9;
const int HD44780_PINS_RS   = 10;
const int HD44780_PINS_E    = 11;
const int HD44780_PINS_DBG  = 12;
const int HD44780_PIN_COUNT = HD44780_MODE;

#define NROW 4
#define ROWLEN 17
#define ROWLENCP (ROWLEN-1)
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
 * - HD44780_PINS_E goes low, committing the instruction
 */

void hd44780_send_data(const int v){ }

void hd44780_send_payload(const int v) {
#if HD44780_CONFIG_DL_DATA_LENGTH == 1
    hd44780_send_data(v);
#elif HD44780_CONFIG_DL_DATA_LENGTH == 0
    hd44780_send_data(get_high_4bits(v));
    hd44780_send_data(get_low_4bits(v));
#endif
}

void hd44780_send_instruction(const int v) {
    //gpio_put( HD44780_PINS_RS, 0 );
    hd44780_send_payload(v);
}

void hd44780_send_data_payload(const int v) {
    //gpio_put( HD44780_PINS_RS, 1 );
    hd44780_send_payload(v);
}

void hd44780_inst_display_clear() {
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
    hd44780_send_instruction(val);
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
}

void hd44780_inst_return_home() {
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
    hd44780_send_instruction(val);
    //vTaskDelayUntil( , hd44780_INST_CLEAR_DISPLAY_MS );
}

#define HD44780_CONFIG_ID_INCREMENT_DIRECTION 1 // 1 - Right | 0 - Left
#define HD44780_CONFIG_SHIFT_CURSOR 1 // 0 off | 1 on 
void hd44780_inst_entry_mode_set(const int id, const int s) {
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
    hd44780_send_instruction(val);
}

void hd44780_inst_display_control(const int d, const int c, const int b) {
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
    hd44780_send_instruction(val);
}

void hd44780_inst_cursor_display_shift(const int sc, const int rl) {
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
    hd44780_send_instruction(val);
}

// This specific call is to transition from 8 bit to 4 bit mode
void hd44780_inst_function_set_half() {
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
    hd44780_send_data(get_high_4bits(val));
}

void hd44780_inst_function_set() {
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
    hd44780_send_instruction(val);
}

void hd44780_inst_set_cgram_address(const int address) {
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
    hd44780_send_instruction(val);
}

void hd44780_inst_set_ddram_address(const int address) {
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
    hd44780_send_instruction(val);
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

// Class functions
test_hd44780::test_hd44780(VerilatedContext& vc) : hd44780(Vhd44780{&vc}) {
    cycle = 0;
    hcycle = 0;
    le = 0;
    hd44780.rst = 0; // Negated so is now acting as reset
    hd44780.clk = 0;
    hd44780.trg = 0;
    // Evaluate to enforce at least one cycle for validity of outputs
    hd44780.eval();
}

std::string test_hd44780::to_string() {
    std::stringstream ss;
    ss << "Cycle: " << cycle;
    ss << " Rst: " << (unsigned int)hd44780.rst;
    ss << " Clk: " << (unsigned int)hd44780.clk;
    ss << " Trg: " << (unsigned int)hd44780.trg;
    ss << " Busy: " << (unsigned int)hd44780.busy;
    ss << " E: " << (unsigned int)hd44780.e;
    ss << " RS: " << (unsigned int)hd44780.rs;
    ss << " DB (h): " << std::hex << (unsigned int)hd44780.db;
    ss << " DB (b): " << std::bitset<4>((unsigned int)hd44780.db);
    return ss.str();
}

void test_hd44780::nextHalfCycle() {
    syncVariables();
    hd44780.clk = hd44780.clk ^ 0x01;
    cycle += hd44780.clk; // Only on full waveform
    hcycle++; // On each edge
    hd44780.eval();
}

void test_hd44780::nextNHalfCycles(const unsigned int cycles) {
    for(unsigned int i=0; i<cycles; i++) {
        nextHalfCycle();
    }
}

void test_hd44780::nextCycle() {
    nextNHalfCycles(2);
}

bool test_hd44780::changeEnable() {
    return (le ^ hd44780.e);
}

bool test_hd44780::transitionEnableHigh() {
    return changeEnable() & hd44780.e;
}

void test_hd44780::setReset(unsigned char reset) {
    hd44780.rst = reset;
}

unsigned long long test_hd44780::getCycles() {
    return cycle;
}

unsigned char test_hd44780::getrst() {
    return hd44780.rst;
}

unsigned char test_hd44780::getclk() {
    return hd44780.clk;
}

unsigned char test_hd44780::gettrg() {
    return hd44780.trg;
}

unsigned char test_hd44780::getbusy() {
    return hd44780.busy;
}

unsigned char test_hd44780::gete() {
    return hd44780.e;
}

unsigned char test_hd44780::getrs() {
    return hd44780.rs;
}

unsigned char test_hd44780::getdb() {
    return hd44780.db;
}

void test_hd44780::syncVariables() {
    this->le = hd44780.e;
}
