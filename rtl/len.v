`timescale 1ns / 1ps


module len (
	input  wire       clk,
    input  wire       resetn,

    output wire       trap,
    
    /* VGA signals */
    output wire       hsync,
    output wire       vsync,
    output wire [3:0] c_r,
    output wire [3:0] c_g,
    output wire [3:0] c_b,

	/* UART signals */
    output wire       ser_tx,
	input  wire       ser_rx
);

	wire		mem_valid;
    wire        mem_instr;
    wire        mem_ready;
    wire [31:0] mem_addr;
    wire [31:0] mem_wdata;
    wire [ 3:0] mem_wstrb;
    wire [31:0] mem_rdata;
    
    wire        ram_mem_valid;
    wire        ram_mem_instr;
    wire        ram_mem_ready;
    wire [31:0] ram_mem_addr;
    wire [31:0] ram_mem_wdata;
    wire [ 3:0] ram_mem_wstrb;
    wire [31:0] ram_mem_rdata;
    
    wire        vga_mem_valid;
    wire        vga_mem_instr;
    wire        vga_mem_ready;
    wire [31:0] vga_mem_addr;
    wire [31:0] vga_mem_wdata;
    wire [ 3:0] vga_mem_wstrb;
    wire [31:0] vga_mem_rdata;

    wire        uart_mem_valid;
    wire        uart_mem_instr;
    wire        uart_mem_ready;
    wire [31:0] uart_mem_addr;
    wire [31:0] uart_mem_wdata;
    wire [ 3:0] uart_mem_wstrb;
    wire [31:0] uart_mem_rdata;

    picorv32 #(
        .STACKADDR(32'h00007FF0)
    ) picorv32_instance (
    	.clk            (clk),
    	.resetn         (resetn),
    	.trap           (trap),

    	.mem_valid      (mem_valid),
    	.mem_instr      (mem_instr),
    	.mem_ready      (mem_ready),
    	.mem_addr       (mem_addr),
    	.mem_wdata      (mem_wdata),
    	.mem_wstrb      (mem_wstrb),
    	.mem_rdata      (mem_rdata)
	);

	mmu mmu_instance(
		.clk            (clk),

		.mem_valid      (mem_valid),
		.mem_instr      (mem_instr),
		.mem_ready      (mem_ready),
		.mem_addr       (mem_addr),
		.mem_wdata      (mem_wdata),
		.mem_wstrb      (mem_wstrb),
		.mem_rdata      (mem_rdata),
	
		.ram_mem_valid  (ram_mem_valid),
		.ram_mem_instr  (ram_mem_instr),
		.ram_mem_ready  (ram_mem_ready),
		.ram_mem_addr   (ram_mem_addr),
		.ram_mem_wdata  (ram_mem_wdata),
		.ram_mem_wstrb  (ram_mem_wstrb),
		.ram_mem_rdata  (ram_mem_rdata),
	
		.vga_mem_valid  (vga_mem_valid),
		.vga_mem_instr  (vga_mem_instr),
		.vga_mem_ready  (vga_mem_ready),
		.vga_mem_addr   (vga_mem_addr),
		.vga_mem_wdata  (vga_mem_wdata),
		.vga_mem_wstrb  (vga_mem_wstrb),
		.vga_mem_rdata  (vga_mem_rdata),

		.uart_mem_valid (uart_mem_valid),
		.uart_mem_instr (uart_mem_instr),
		.uart_mem_ready (uart_mem_ready),
		.uart_mem_addr  (uart_mem_addr),
		.uart_mem_wdata (uart_mem_wdata),
		.uart_mem_wstrb (uart_mem_wstrb),
		.uart_mem_rdata (uart_mem_rdata)
	);
	
	ram ram_instance (
		.clk            (clk),
		.resetn         (resetn),

		.ram_mem_valid  (ram_mem_valid),
		.ram_mem_instr  (ram_mem_instr),
		.ram_mem_ready  (ram_mem_ready),
		.ram_mem_addr   (ram_mem_addr),
		.ram_mem_wdata  (ram_mem_wdata),
		.ram_mem_wstrb  (ram_mem_wstrb),
		.ram_mem_rdata  (ram_mem_rdata)
	);
	
	vga vga_instance (
		.clk            (clk),
		.resetn         (resetn),

		.hsync          (hsync),
		.vsync          (vsync),
		.c_r            (c_r),
		.c_g            (c_g),
		.c_b            (c_b),
	
		.vga_mem_valid  (vga_mem_valid),
		.vga_mem_instr  (vga_mem_instr),
		.vga_mem_ready  (vga_mem_ready),
		.vga_mem_addr   (vga_mem_addr),
		.vga_mem_wdata  (vga_mem_wdata),
		.vga_mem_wstrb  (vga_mem_wstrb),
		.vga_mem_rdata  (vga_mem_rdata)
	);

	uart #(
		.CLKS_PER_BIT(5208)
	) uart_instance (
		.clk            (clk),
		.resetn         (resetn),

		.ser_tx         (ser_tx),
		.ser_rx         (ser_rx),
		
		.uart_mem_valid (uart_mem_valid),
		.uart_mem_instr (uart_mem_instr),
		.uart_mem_ready (uart_mem_ready),
		.uart_mem_addr  (uart_mem_addr),
		.uart_mem_wdata (uart_mem_wdata),
		.uart_mem_wstrb (uart_mem_wstrb),
		.uart_mem_rdata (uart_mem_rdata)
	);

endmodule
