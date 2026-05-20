`timescale 1ns / 1ps

module myip_camera_capture (
    input  wire       clk,

    // OV7670
    input  wire [7:0] cam_d,
    input  wire       cam_vsync,
    input  wire       cam_pclk,
    input  wire       cam_href,
    output wire       cam_scl,
    inout  wire       cam_sda,
    output wire       cam_rst,
    output wire       cam_xclk,
    output wire       cam_pwdn,

    // VGA
    output wire [3:0] vgaRed,
    output wire [3:0] vgaGreen,
    output wire [3:0] vgaBlue,
    output wire       Hsync,
    output wire       Vsync,

    // shape_classifier IP로 연결
    output wire [7:0] sampled_pixel,
    output wire       sampled_valid,
    output wire       frame_done,
    output wire       clk_25mhz_out
);

    myip_camera_capture_v1_0 u_cam (
        .clk           (clk),
        .cam_d         (cam_d),
        .cam_vsync     (cam_vsync),
        .cam_pclk      (cam_pclk),
        .cam_href      (cam_href),
        .cam_scl       (cam_scl),
        .cam_sda       (cam_sda),
        .cam_rst       (cam_rst),
        .cam_xclk      (cam_xclk),
        .cam_pwdn      (cam_pwdn),
        .vgaRed        (vgaRed),
        .vgaGreen      (vgaGreen),
        .vgaBlue       (vgaBlue),
        .Hsync         (Hsync),
        .Vsync         (Vsync),
        .sampled_pixel (sampled_pixel),
        .sampled_valid (sampled_valid),
        .frame_done    (frame_done),
        .clk_25mhz_out (clk_25mhz_out)
    );

endmodule