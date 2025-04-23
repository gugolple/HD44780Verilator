#ifndef HD44780GENERAL_HPP
#define HD44780GENERAL_HPP
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
#define hd44780_POWERON_DELAY_MS      ( 100 / portTICK_PERIOD_MS )
#define hd44780_INST_CLEAR_DISPLAY_MS ( 10 / portTICK_PERIOD_MS )
#define hd44780_INST_DELAY_US         80

#define HD44780_START_ADD_L1          (0x00) 
#define HD44780_START_ADD_L2          (0x40)
#define HD44780_START_ADD_L3          (0x10) 
#define HD44780_START_ADD_L4          (0x50)

void reset_sequence();
#endif
