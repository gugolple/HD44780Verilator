#ifndef HD44780GENERAL_HPP
#define HD44780GENERAL_HPP

#include <sstream>

// Hardware configuration of unit
#define HD44780_CONFIG_DL_DATA_LENGTH   0 //0 - 4 | 1 - 8 //Bits for comm
#define HD44780_CONFIG_N_DISPLAY_LINES  1 //0 - 1 | 1 - 2
#define HD44780_CONFIG_F_CHARACTER_FONT 0 

// Display configuration
#define HD44780_CONFIG_D_DISPLAY_ONOFF  1 // 0 - Off | 1 - On
#define HD44780_CONFIG_C_CURSOR_ONOFF   1 // 0 - Off | 1 - On
#define HD44780_CONFIG_C_CURSOR_BLINK   0 // 0 - Off | 1 - On

// Display cursor configuration
#define HD44780_CONFIG_ID_INCREMENT_DIRECTION 1 // 0 - Left | 1 - Right
#define HD44780_CONFIG_SHIFT_CURSOR           1 // 0 Off | 1 on 

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
#define HD44780_POWERON_DELAY_MS      100
#define HD44780_INST_CLEAR_DISPLAY_MS 10
#define HD44780_INST_DELAY_US         80

#define HD44780_START_ADD_L1          (0x00) 
#define HD44780_START_ADD_L2          (0x40)
#define HD44780_START_ADD_L3          (HD44780_START_ADD_L1+ROWLENACT) 
#define HD44780_START_ADD_L4          (HD44780_START_ADD_L2+ROWLENACT)

// Pins assigned for the HD44780 interface
// Only 4 high used if set to 4 bit mode at HD44780_MODE
#if HD44780_CONFIG_DL_DATA_LENGTH == 1
#define HD44780_MODE 8
#elif HD44780_CONFIG_DL_DATA_LENGTH == 0
#define HD44780_MODE 4
#else
#error INVALID HD44780_CONFIG_DL_DATA_LENGTH MUST BE EITHER 0 or 1
#endif

#define NROW 4
#define ROWLEN 21
#define ROWLENACT 20

/* 
 * All operations are:
 * - HD44780_PINS_E goes high
 * - HD44780_PINS_DATA gets set
 * - Wait until the 1/500k for the clock to elapse the time
 * - HD44780_PINS_E goes low, committing the instruction
 */
#if HD44780_CONFIG_DL_DATA_LENGTH == 1
class HD44780Payload {
    public:
        HD44780Payload(int payload, bool rss);
        int getbits() const;
        int getrs() const;
    private:
        bool rs;
        int bits;
};

#elif HD44780_CONFIG_DL_DATA_LENGTH == 0
class HD44780Payload {
    public:
        HD44780Payload(int payload, bool rss);
        HD44780Payload(int payload, bool rss, bool oh);
        int getrs() const;
        int gethighbits() const;
        int getlowbits() const;
        std::string to_string() const;

    private:
        bool rs;
        bool onlyhigh;
        int highbits;
        int lowbits;
};
#endif

HD44780Payload hd44780_inst_display_clear();
HD44780Payload hd44780_inst_return_home();
HD44780Payload hd44780_inst_entry_mode_set(const int id, const int s);
HD44780Payload hd44780_inst_display_control(const int d, const int c, const int b);
HD44780Payload hd44780_inst_cursor_display_shift(const int sc, const int rl);
HD44780Payload hd44780_inst_function_set_half();
HD44780Payload hd44780_inst_function_set();
HD44780Payload hd44780_inst_set_cgram_address(const int address);
HD44780Payload hd44780_inst_set_ddram_address(const int address);
HD44780Payload hd44780_inst_set_ddram_l1();
HD44780Payload hd44780_inst_set_ddram_l2();
HD44780Payload hd44780_inst_set_ddram_l3();
HD44780Payload hd44780_inst_set_ddram_l4();
#endif
