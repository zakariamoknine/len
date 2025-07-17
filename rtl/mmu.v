`timescale 1ns / 1ps


module mmu (
	input  wire        clk,

	input  wire        mem_valid,
	input  wire        mem_instr,
	output reg         mem_ready,
	input  wire [31:0] mem_addr,
	input  wire [31:0] mem_wdata,
	input  wire [ 3:0] mem_wstrb,
	output reg  [31:0] mem_rdata,
	
	output reg         ram_mem_valid,
	output reg         ram_mem_instr,
	input  wire        ram_mem_ready,
	output reg  [31:0] ram_mem_addr,
	output reg  [31:0] ram_mem_wdata,
	output reg  [ 3:0] ram_mem_wstrb,
	input  wire [31:0] ram_mem_rdata,

	output reg         vga_mem_valid,
	output reg         vga_mem_instr,
	input  wire        vga_mem_ready,
	output reg  [31:0] vga_mem_addr,
	output reg  [31:0] vga_mem_wdata,
	output reg  [ 3:0] vga_mem_wstrb,
	input  wire [31:0] vga_mem_rdata,

	output reg         uart_mem_valid,
	output reg         uart_mem_instr,
	input  wire        uart_mem_ready,
	output reg  [31:0] uart_mem_addr,
	output reg  [31:0] uart_mem_wdata,
	output reg  [ 3:0] uart_mem_wstrb,
	input  wire [31:0] uart_mem_rdata
);

	always @(*) begin
		ram_mem_valid  = 0;
		ram_mem_instr  = 0;
		ram_mem_addr   = 0;
		ram_mem_wdata  = 0;
		ram_mem_wstrb  = 0;

		vga_mem_valid  = 0;
		vga_mem_instr  = 0;
		vga_mem_addr   = 0;
		vga_mem_wdata  = 0;
		vga_mem_wstrb  = 0;

		uart_mem_valid = 0;
		uart_mem_instr = 0;
		uart_mem_addr  = 0;
		uart_mem_wdata = 0;
		uart_mem_wstrb = 0;

		mem_ready      = 0;
		mem_rdata      = 0;

		if (mem_valid) begin
			if (mem_addr <= 32'h00007FFF) begin
				ram_mem_valid  = mem_valid;
				ram_mem_instr  = mem_instr;
				ram_mem_addr   = mem_addr;
				ram_mem_wdata  = mem_wdata;
				ram_mem_wstrb  = mem_wstrb;

				mem_ready      = ram_mem_ready;
				mem_rdata      = ram_mem_rdata;

			end else if (mem_addr >= 32'h8FFFF6A0 && mem_addr <= 32'h8FFFFFFF) begin
				vga_mem_valid  = mem_valid;
				vga_mem_instr  = mem_instr;
				vga_mem_addr   = mem_addr - 32'h8FFFF6A0;
				vga_mem_wdata  = mem_wdata;
				vga_mem_wstrb  = mem_wstrb;

				mem_ready      = vga_mem_ready;
				mem_rdata      = vga_mem_rdata;

			end else if (mem_addr >= 32'h80000000 && mem_addr <= 32'h800000FF) begin
				uart_mem_valid = mem_valid;
				uart_mem_instr = mem_instr;
				uart_mem_addr  = mem_addr - 32'h80000000;
				uart_mem_wdata = mem_wdata;
				uart_mem_wstrb = mem_wstrb;

				mem_ready      = uart_mem_ready;
				mem_rdata      = uart_mem_rdata;

			end else begin
				ram_mem_valid  = 0;
				ram_mem_instr  = 0;
				ram_mem_addr   = 0;
				ram_mem_wdata  = 0;
				ram_mem_wstrb  = 0;

				vga_mem_valid  = 0;
				vga_mem_instr  = 0;
				vga_mem_addr   = 0;
				vga_mem_wdata  = 0;
				vga_mem_wstrb  = 0;

				uart_mem_valid = 0;
				uart_mem_instr = 0;
				uart_mem_addr  = 0;
				uart_mem_wdata = 0;
				uart_mem_wstrb = 0;

				mem_ready      = 0;
				mem_rdata      = 0;

			end
		end
	end

endmodule
