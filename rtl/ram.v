`timescale 1ns / 1ps


module ram (
	input  wire        clk,
	input  wire        resetn,
	
	input  wire        ram_mem_valid,
	input  wire        ram_mem_instr,
	output reg         ram_mem_ready,
	input  wire [31:0] ram_mem_addr,
	input  wire [31:0] ram_mem_wdata,
	input  wire [ 3:0] ram_mem_wstrb,
	output reg  [31:0] ram_mem_rdata
);

    /* 32KB general-purpose RAM */
	reg [31:0] mem [0:8191];
	initial $readmemh("program.mem", mem);

	always @(posedge clk) begin
		if (!resetn) begin
			ram_mem_ready <= 0;
			ram_mem_rdata <= 0;
		end else begin
			ram_mem_ready <= 0;
			if (ram_mem_valid && !ram_mem_ready) begin
				ram_mem_rdata <= mem[ram_mem_addr >> 2];
				if (ram_mem_wstrb[0]) mem[ram_mem_addr >> 2][ 7: 0] <= ram_mem_wdata[ 7: 0];
				if (ram_mem_wstrb[1]) mem[ram_mem_addr >> 2][15: 8] <= ram_mem_wdata[15: 8];
				if (ram_mem_wstrb[2]) mem[ram_mem_addr >> 2][23:16] <= ram_mem_wdata[23:16];
				if (ram_mem_wstrb[3]) mem[ram_mem_addr >> 2][31:24] <= ram_mem_wdata[31:24];
				ram_mem_ready <= 1;
			end
		end
	end

endmodule
