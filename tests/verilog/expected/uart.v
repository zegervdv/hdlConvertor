//http://www.asic-world.com/examples/verilog/uart.html
//-----------------------------------------------------
// Design Name : uart 
// File Name   : uart.v
// Function    : Simple UART
// Coder       : Deepak Kumar Tala
//-----------------------------------------------------
module uart (
    // Port declarations
    input reset,
    input txclk,
    input ld_tx_data,
    input [7:0] tx_data,
    input tx_enable,
    output tx_out,
    output tx_empty,
    input rxclk,
    input uld_rx_data,
    output [7:0] rx_data,
    input rx_enable,
    input rx_in,
    output rx_empty
);
    // Internal Variables 
    reg [7:0] tx_reg;
    reg tx_empty;
    reg tx_over_run;
    reg [3:0] tx_cnt;
    reg tx_out;
    reg [7:0] rx_reg;
    reg [7:0] rx_data;
    reg [3:0] rx_sample_cnt;
    reg [3:0] rx_cnt;
    reg rx_frame_err;
    reg rx_over_run;
    reg rx_empty;
    reg rx_d1;
    reg rx_d2;
    reg rx_busy;
    always @ ((posedge rxclk or posedge reset)) begin
        if (reset) begin
            rx_reg <= 0;
            rx_data <= 0;
            rx_sample_cnt <= 0;
            rx_cnt <= 0;
            rx_frame_err <= 0;
            rx_over_run <= 0;
            rx_empty <= 1;
            rx_d1 <= 1;
            rx_d2 <= 1;
            rx_busy <= 0;
        end else begin
            // Synchronize the asynch signal
            rx_d1 <= rx_in;
            rx_d2 <= rx_d1;
            // Uload the rx data
            if (uld_rx_data) begin
                rx_data <= rx_reg;
                rx_empty <= 1;
            end
            // Receive data only when rx is enabled
            if (rx_enable) begin
                // Check if just received start of frame
                if ((!rx_busy && !rx_d2)) begin
                    rx_busy <= 1;
                    rx_sample_cnt <= 1;
                    rx_cnt <= 0;
                end
                // Start of frame detected, Proceed with rest of data
                if (rx_busy) begin
                    rx_sample_cnt <= (rx_sample_cnt + 1);
                    // Logic to sample at middle of data
                    if ((rx_sample_cnt == 7))
                        if (((rx_d2 == 1) && (rx_cnt == 0)))
                            rx_busy <= 0;
                        else begin
                            rx_cnt <= (rx_cnt + 1);
                            // Start storing the rx data
                            if ((((rx_cnt > 0) && rx_cnt) < 9))
                                rx_reg[(rx_cnt - 1)] <= rx_d2;
                            if ((rx_cnt == 9)) begin
                                rx_busy <= 0;
                                // Check if End of frame received correctly
                                if ((rx_d2 == 0))
                                    rx_frame_err <= 1;
                                else begin
                                    rx_empty <= 0;
                                    rx_frame_err <= 0;
                                    // Check if last rx data was not unloaded,
                                    rx_over_run <= rx_empty ? 0 : 1;
                                endend
                        endend
            end
            if (!rx_enable)
                rx_busy <= 0;
        end
    end
    always @ ((posedge txclk or posedge reset)) begin
        if (reset) begin
            tx_reg <= 0;
            tx_empty <= 1;
            tx_over_run <= 0;
            tx_out <= 1;
            tx_cnt <= 0;
        end else begin
            if (ld_tx_data)
                if (!tx_empty)
                    tx_over_run <= 0;
                else begin
                    tx_reg <= tx_data;
                    tx_empty <= 0;
                endif ((tx_enable && !tx_empty)) begin
                tx_cnt <= (tx_cnt + 1);
                if ((tx_cnt == 0))
                    tx_out <= 0;
                if ((((tx_cnt > 0) && tx_cnt) < 9))
                    tx_out <= tx_reg[(tx_cnt - 1)];
                if ((tx_cnt == 9)) begin
                    tx_out <= 1;
                    tx_cnt <= 0;
                    tx_empty <= 1;
                end
            end
            if (!tx_enable)
                tx_cnt <= 0;
        end
    end
endmodule
