`timescale 1ns / 1ps


module vga (
    input  wire        clk,
    input  wire        resetn,
     
    output wire        hsync,
    output wire        vsync,
    output wire [ 3:0] c_r,
    output wire [ 3:0] c_g,
    output wire [ 3:0] c_b,

    input  wire        vga_mem_valid,
    input  wire        vga_mem_instr,
    output reg         vga_mem_ready,
    input  wire [31:0] vga_mem_addr,
    input  wire [31:0] vga_mem_wdata,
    input  wire [ 3:0] vga_mem_wstrb,
    output reg  [31:0] vga_mem_rdata
);

    // 640x480 @60Hz
    localparam H_VISIBLE = 640;
    localparam H_FRONT   = 16;
    localparam H_SYNC    = 96;
    localparam H_BACK    = 48;
    localparam H_TOTAL   = H_VISIBLE + H_FRONT + H_SYNC + H_BACK;

    localparam V_VISIBLE = 480;
    localparam V_FRONT   = 10;
    localparam V_SYNC    = 2;
    localparam V_BACK    = 33;
    localparam V_TOTAL   = V_VISIBLE + V_FRONT + V_SYNC + V_BACK;

	/* VGA 80x30 text mode framebuffer */
	reg [31:0] vga_mem [0:599];

	/* ASCII font rom */
	reg [7:0] font8x16_rom [0:4095];
	initial $readmemh("font_vga_8x16.mem", font8x16_rom);
	
	/* clk is 100MHz, VGA requires 25MHz, divide clk by 4 */
    reg [1:0] count_clk_div;
    reg clk_vga;
    always @(posedge clk) begin
        count_clk_div <= count_clk_div + 1;
		clk_vga <= (count_clk_div == 2'b11);
    end

    reg [9:0] hcount;
    reg [9:0] vcount;

    wire [6:0] char_col = hcount[9:3];
    wire [4:0] char_row = vcount[8:4];

    wire [2:0] font_x = hcount[2:0];
    wire [3:0] font_y = vcount[3:0];
    
	wire [12:0] char_index = char_row * 80 + char_col;
    wire [31:0] word_data = vga_mem[char_index >> 2];
    wire [1:0]  byte_offset = char_index[1:0];
    wire [7:0] char_data = (byte_offset == 2'b00) ? word_data[7:0]   :
                           (byte_offset == 2'b01) ? word_data[15:8]  :
                           (byte_offset == 2'b10) ? word_data[23:16] :
                                                    word_data[31:24];

    wire [11:0] font_index = {char_data, font_y};
    wire [7:0] font_line = font8x16_rom[font_index];

    wire pixel_on = font_line[~font_x];
    wire visible = (hcount < H_VISIBLE) && (vcount < V_VISIBLE);

	always @(posedge clk) begin
		if (!resetn) begin
			vga_mem_ready <= 0;
			vga_mem_rdata <= 0;
			hcount <= 0;
			vcount <= 0;
		end else begin
        	vga_mem_ready <= 0;
        	if (vga_mem_valid && !vga_mem_ready) begin
        	    vga_mem_rdata <= vga_mem[vga_mem_addr >> 2];
        	    if (vga_mem_wstrb[0]) vga_mem[vga_mem_addr >> 2][ 7: 0] <= vga_mem_wdata[ 7: 0];
        	    if (vga_mem_wstrb[1]) vga_mem[vga_mem_addr >> 2][15: 8] <= vga_mem_wdata[15: 8];
        	    if (vga_mem_wstrb[2]) vga_mem[vga_mem_addr >> 2][23:16] <= vga_mem_wdata[23:16];
        	    if (vga_mem_wstrb[3]) vga_mem[vga_mem_addr >> 2][31:24] <= vga_mem_wdata[31:24];
        	    vga_mem_ready <= 1;
			end
        	if (clk_vga) begin
            	if (hcount == H_TOTAL - 1) begin
            	    hcount <= 0;
            	    if (vcount == V_TOTAL - 1)
            	        vcount <= 0;
            	    else
            	        vcount <= vcount + 1;
            	end else begin
            	    hcount <= hcount + 1;
            	end
        	end
		end
	end

    assign hsync = ~((hcount >= H_VISIBLE + H_FRONT) && (hcount <  H_VISIBLE + H_FRONT + H_SYNC));
    assign vsync = ~((vcount >= V_VISIBLE + V_FRONT) && (vcount <  V_VISIBLE + V_FRONT + V_SYNC));
    assign c_r = (visible && pixel_on) ? 4'hF : 4'h0;
    assign c_g = (visible && pixel_on) ? 4'hF : 4'h0;
    assign c_b = (visible && pixel_on) ? 4'hF : 4'h0;

endmodule
