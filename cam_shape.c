#include "cam_shape.h"
#include "xil_io.h"
#include "xil_printf.h"

int CamShape_Read(void)
{
    u32 reg = Xil_In32(CAM_SHAPE_BASE) & 0xFu;

    switch (reg) {
        case CAM_NONE_WHITE:
            xil_printf("[CAM] White background (NONE)\r\n");
            return -1;
        case CAM_NONE_BLACK:
            xil_printf("[CAM] Black background (BLOCKED)\r\n");
            return -1;
        case CAM_UNKNOWN:
            xil_printf("[CAM] Detecting... (UNKNOWN)\r\n");
            return -1;
        case CAM_CIRCLE:
            xil_printf("[CAM] O Circle detected \r\n");
            return (int)CAM_CIRCLE;
        case CAM_RECT:
            xil_printf("[CAM] [] Square detected \r\n");
            return (int)CAM_RECT;
        case CAM_CROSS:
            xil_printf("[CAM] +x O Cross detected \r\n");
            return (int)CAM_CROSS;
        default:
            xil_printf("[CAM] Unknown signal 0x%X\r\n", (unsigned int)reg);
            return -1;
    }
}

int CamShape_IsValid(int shape_code)
{
    return (shape_code == (int)CAM_CIRCLE ||
            shape_code == (int)CAM_RECT   ||
            shape_code == (int)CAM_CROSS);
}

void CamShape_ToGrid(int shape_code, int *row, int *col)
{
    switch ((u32)shape_code) {
        case CAM_CIRCLE: *row = 0; *col = 0; break;  /* 동작1 */
        case CAM_RECT:   *row = 0; *col = 2; break;  /* 동작2 */
        case CAM_CROSS:  *row = 2; *col = 1; break;  /* 동작3 */
        default:         *row = 1; *col = 1; break;  /* 중앙  */
    }
}

void CamShape_ToPickPos(int shape_code, int *row, int *col)
{
    switch ((u32)shape_code) {
        case CAM_CIRCLE: *row = PICK_CIRCLE_ROW; *col = PICK_CIRCLE_COL; break;
        case CAM_RECT:   *row = PICK_RECT_ROW;   *col = PICK_RECT_COL;   break;
        case CAM_CROSS:  *row = PICK_CROSS_ROW;  *col = PICK_CROSS_COL;  break;
        default:         *row = 2;               *col = 1;               break;
    }
}