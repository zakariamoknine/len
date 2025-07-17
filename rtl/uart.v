`timescale 1ns / 1ps


module uart #(
	parameter CLKS_PER_BIT = 5208
) (
	input  wire        clk,
	input  wire        resetn,
	
	output wire        ser_tx,
	input  wire        ser_rx,
	
	input  wire        uart_mem_valid,
	input  wire        uart_mem_instr,
	output reg         uart_mem_ready,
	input  wire [31:0] uart_mem_addr,
	input  wire [31:0] uart_mem_wdata,
	input  wire [ 3:0] uart_mem_wstrb,
	output reg  [31:0] uart_mem_rdata
);

	wire [7:0] rx_data;
	wire       rx_valid;
	reg  [7:0] rx_data_latch;
	reg        rx_valid_latch;
	
	reg  [7:0] tx_data;
	reg        tx_start;
	wire       tx_busy;
	wire       tx_done;

	uart_rx #(
		.CLKS_PER_BIT(CLKS_PER_BIT)
	) uart_rx_instance (
		.clk      (clk),
		.resetn   (resetn),
		
		.rx       (ser_rx),
		.rx_data  (rx_data),
		.rx_valid (rx_valid)
	);

	uart_tx #(
		.CLKS_PER_BIT(CLKS_PER_BIT)
	) uart_tx_instance (
		.clk      (clk),
		.resetn   (resetn),

		.tx_data  (tx_data),
		.tx_start (tx_start),
		.tx       (ser_tx),
		.tx_busy  (tx_busy),
		.tx_done  (tx_done)
	);

	always @(posedge clk) begin
		if (!resetn) begin
			tx_start <= 0;
			tx_data <= 0;
			rx_data_latch <= 0;
			rx_valid_latch <= 0;
			uart_mem_ready <= 0;
			uart_mem_rdata <= 0;
		end else begin
			if (rx_valid) begin
    			rx_data_latch <= rx_data;
				rx_valid_latch <= 1;
			end
			uart_mem_ready <= 0;
			if (uart_mem_valid && !uart_mem_ready) begin
				case (uart_mem_addr[3:0])
					4'h0: begin
						uart_mem_rdata <= 32'b0;
    					if      (uart_mem_wstrb[0]) tx_data <= uart_mem_wdata[ 7: 0];
    					else if (uart_mem_wstrb[1]) tx_data <= uart_mem_wdata[15: 8];
    					else if (uart_mem_wstrb[2]) tx_data <= uart_mem_wdata[23:16];
    					else if (uart_mem_wstrb[3]) tx_data <= uart_mem_wdata[31:24];
						else                        tx_data <= 0;
						tx_start <= 1;
					end

					4'h4: begin
						uart_mem_rdata <= {24'b0, rx_data_latch};
						rx_valid_latch <= 0;
						tx_start <= 0;
					end

					4'h8: begin
						uart_mem_rdata <= {30'b0, ~tx_busy, rx_valid_latch};
						tx_start <= 0;
					end

					default: begin
						uart_mem_rdata <= 32'h0;
						tx_start <= 0;
					end

				endcase
				uart_mem_ready <= 1;
			end else begin
    		    tx_start <= 0;
			end
		end
	end

endmodule


module uart_rx #(
	parameter CLKS_PER_BIT = 5208
) (
	input  wire       clk,
	input  wire       resetn,
	
	input  wire       rx,
	output reg [7:0]  rx_data,
	output reg        rx_valid
);

	localparam RX_IDLE    = 2'b00;
	localparam RX_START   = 2'b01;
	localparam RX_DATA    = 2'b10;
	localparam RX_STOP    = 2'b11;
	
	reg [1:0] state;
	reg [15:0] count;
	reg [2:0] bit_index;
	reg [7:0] rx_shift;
	
	reg rx_buf1;
	reg rx_buf2;
	wire i_rx = rx_buf2;
	
	always @(posedge clk) begin
		if (!resetn) begin
			rx_data <= 0;
			rx_valid <= 0;
			state <= RX_IDLE;
			count <= 0;
			bit_index <= 0;
			rx_shift <= 0;
			rx_buf1 <= 1;
			rx_buf2 <= 1;
		end else begin
			rx_buf1 <= rx;
			rx_buf2 <= rx_buf1;
			case (state)
				RX_IDLE: begin
					rx_valid  <= 0;
					count     <= 0;
					rx_shift  <= 0;
					bit_index <= 0;
					
					if (i_rx == 0) begin
						state <= RX_START;
					end else begin
						state <= RX_IDLE;
					end
				end
				
				RX_START: begin
					if (count == (CLKS_PER_BIT / 2)) begin
						if (i_rx == 0) begin
							count <= 0;
							state <= RX_DATA;
						end else begin
							state <= RX_IDLE;
						end
					end else begin
						count <= count + 1;
					end
				end
				
				RX_DATA: begin
					if (count < CLKS_PER_BIT - 1) begin
						count <= count + 1;
					end else begin
						count <= 0;
						rx_shift[bit_index] <= i_rx;
						
						if (bit_index < 7) begin
						    bit_index <= bit_index + 1;
						end else begin
						    bit_index <= 0;
						    state <= RX_STOP;
						end
					end
				end
				
				RX_STOP: begin
				    if (count < CLKS_PER_BIT - 1) begin
				        count <= count + 1;
				    end else begin
				        rx_data  <= rx_shift;
				        rx_valid <= 1;
				        count    <= 0;
				        state    <= RX_IDLE;
				    end
				end
				
				default: begin
				    state <= RX_IDLE;
				end
			
			endcase
		end
	end
	
endmodule


module uart_tx #(
	parameter CLKS_PER_BIT = 5208
) (
	input  wire       clk,
	input  wire       resetn,
	
	input  wire [7:0] tx_data,
	input  wire       tx_start,
	output reg        tx,
	output reg        tx_busy,
	output reg        tx_done
);

	localparam TX_IDLE  = 2'b00;
	localparam TX_START = 2'b01;
	localparam TX_DATA  = 2'b10;
	localparam TX_STOP  = 2'b11;
	
	reg [1:0] state;
	reg [15:0] count;
	reg [2:0] bit_index;
	reg [7:0] tx_shift;
	
	always @(posedge clk) begin
		if (!resetn) begin
			tx <= 1;
			tx_busy <= 0;
			tx_done <= 0;
			state <= TX_IDLE;
			count <= 0;
			bit_index <= 0;
			tx_shift <= 0;
		end else begin
			case (state)
				TX_IDLE: begin
					tx      <= 1;
					tx_done <= 0;
					tx_busy <= 0;
					count   <= 0;
					bit_index <= 0;
		
					if (tx_start) begin
					    tx_shift <= tx_data;
					    tx_busy  <= 1;
					    state    <= TX_START;
					end
				end
				
				TX_START: begin
					tx <= 0;
					
					if (count < CLKS_PER_BIT - 1) begin
					    count <= count + 1;
					end else begin
					    count <= 0;
					    state <= TX_DATA;
					end
				end
				
				TX_DATA: begin
					tx <= tx_shift[bit_index];
					
					if (count < CLKS_PER_BIT - 1) begin
					    count <= count + 1;
					end else begin
					    count <= 0;
					
					    if (bit_index < 7) begin
					        bit_index <= bit_index + 1;
					    end else begin
					        bit_index <= 0;
					        state <= TX_STOP;
					    end
					end
				end
				
				TX_STOP: begin
					tx <= 1;
					
					if (count < CLKS_PER_BIT - 1) begin
					    count <= count + 1;
					end else begin
					    count   <= 0;
					    tx_done <= 1;
					    tx_busy <= 0;
					    state   <= TX_IDLE;
					end
				end
				
				default: begin
					state <= TX_IDLE;
				end
			endcase
		end
	end

endmodule
