// This block will receive a divided clock
`define CLOCK_FREQ 500000
//`define PERIOD 1/`CLOCK_FREQ
`define REG_SIZE 8
`define REG_LIM `REG_SIZE-1
`define LINE_SIZE 16
`define LINE_SIZE_LIM `LINE_SIZE-1
module hd44780(
        input clock, 
        input trigger,
        input reset,
        input [`LINE_SIZE_LIM:0] line1,
        input [`LINE_SIZE_LIM:0] line2,
        input [`LINE_SIZE_LIM:0] line3,
        input [`LINE_SIZE_LIM:0] line4,
        output reg rw,
        output reg rs,
        output reg e,
        output reg rdy
    );

    reg v = 0;
    reg [`REG_LIM:0] fn1 = 1;

    always @(posedge clock) begin
        e <= v;
        v <= !v;
    end

    initial begin
        rw <= 0;
        rs <= 0;
        e <= 0;
        rdy <= 1;
        $display("HD44780 controller");
    end
endmodule
