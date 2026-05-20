/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

// src/
// ├── helloworld.c        ← main, UART 루프
// ├── robot_config.h      ← 상수, 타입 정의
// ├── servo.h / .c        ← 서보 각도/duty/하드웨어
// ├── motion.h / .c       ← 집기/놓기 동작 시퀀스
// ├── state_machine.h/.c  ← SM 상태 전이 및 업데이트
// └── cam_shape.h / .c    ← 카메라 도형 읽기

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xuartlite_l.h"
#include "sleep.h"

#include "robot_config.h"
#include "servo.h"
#include "state_machine.h"

/* ============================================================
 * UART 명령 처리
 * ============================================================ */
static int Str_Eq_IgnCase(const char *a, const char *b)
{
    int i = 0;
    while (a[i] && b[i]) {
        char ca = a[i], cb = b[i];
        if (ca >= 'a' && ca <= 'z') ca -= (char)32;
        if (cb >= 'a' && cb <= 'z') cb -= (char)32;
        if (ca != cb) return 0;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

static float Clamp(int idx, float deg)
{
    return Servo_ClampAngle(idx, deg);
}

static void Print_Help(void)
{
    xil_printf("\r\n========== Help ==========\r\n");
    xil_printf("H / HOME : 즉시 홈 복귀 (ACT 중단)\r\n");
    xil_printf("S / STOP : 정지 (현재 위치 유지)\r\n");
    xil_printf("R        : 정지 해제 -> IDLE\r\n");
    xil_printf("M        : 이 도움말\r\n");
    xil_printf("1~9      : 수동 그리드 (IDLE/STOP 한정)\r\n");
    xil_printf("  1[0,0] 2[0,1] 3[0,2]\r\n");
    xil_printf("  4[1,0] 5[1,1] 6[1,2]\r\n");
    xil_printf("  7[2,0] 8[2,1] 9[2,2]\r\n");
    xil_printf("ARC : 강제 ACT, R=row, C=col (IDLE/STOP 한정)\r\n");
    xil_printf("  예: A01=ForceAct(0,1), A22=ForceAct(2,2)\r\n");
    xil_printf("NXX : 서보N을 XX도로 (IDLE/STOP 한정)\r\n");
    xil_printf("  예: 590=서보5->90도, 6127=서보6->127도\r\n");
    xil_printf("CAM auto:\r\n");
    xil_printf("  O -> Grid[0,0]\r\n");
    xil_printf("  [] -> Grid[0,2]\r\n");
    xil_printf("  +x -> Grid[2,1]\r\n");
    xil_printf("==========================\r\n");
}

static void Process_Command(char *cmd)
{
    if (cmd[0] == '\0') return;

    /* H / HOME: 어느 상태에서든 즉시 홈 */
    if (Str_Eq_IgnCase(cmd, "H") || Str_Eq_IgnCase(cmd, "HOME")) {
        SM_CmdHome();
        return;
    }

    /* S / STOP: ACT, HOME, READY 상태에서만 유효 */
    if (Str_Eq_IgnCase(cmd, "S") || Str_Eq_IgnCase(cmd, "STOP")) {
        RobotState st = SM_GetState();
        if (st == STATE_IDLE || st == STATE_STOP) {
            xil_printf("[CMD] S: nothing to stop\r\n");
        } else {
            SM_CmdStop();
        }
        return;
    }

    /* R: 정지 해제 */
    if (Str_Eq_IgnCase(cmd, "R")) {
        SM_CmdResume();
        return;
    }

    /* Y: 확인 */
    if (Str_Eq_IgnCase(cmd, "Y")) {
        SM_CmdConfirm();
        return;
    }

    /* N: 취소 */
    if (Str_Eq_IgnCase(cmd, "N")) {
        SM_CmdCancel();
        return;
    }

    /* M: 도움말 */
    if (Str_Eq_IgnCase(cmd, "M")) {
        Print_Help();
        return;
    }

    /* ARC: 디버깅용 강제 ACT, 예) A01 -> ForceAct(0,1) */
    if (cmd[0] == 'A' || cmd[0] == 'a') {
        if (cmd[1] >= '0' && cmd[1] <= '9' && cmd[2] >= '0' && cmd[2] <= '9' && cmd[3] == '\0') {
            int row = cmd[1] - '0';
            int col = cmd[2] - '0';
            SM_CmdForceAct(row, col);
        } else {
            xil_printf("[CMD] A: 형식 오류. 예) A01 = ForceAct(0,1)\r\n");
        }
        return;
    }

    /* ACT/HOME/READY 중 수동 명령 차단 */
    RobotState st = SM_GetState();
    if (st == STATE_ACT || st == STATE_HOME ) { // || st == STATE_READY
        xil_printf("[CMD] Busy or waiting confirm, ignored\r\n");
        // /* READY 상태에서 지정 cmd 외 입력 → IDLE */
        // if (st == STATE_READY) {
        //     xil_printf("[CMD] Unknown input -> IDLE\r\n");
        //     SM_CmdCancel();
        // }
        return;
    }

    /* 1~9: 수동 그리드 */
    if (cmd[0] >= '1' && cmd[0] <= '9' && cmd[1] == '\0') {
        int n   = cmd[0] - '0';
        int row = (n - 1) / GRID_SIZE;
        int col = (n - 1) % GRID_SIZE;
        SM_CmdManualGrid(row, col);
        return;
    }

    /* NXX: 서보 각도 직접 지정 */
    if (cmd[0] >= '1' && cmd[0] <= '6' && cmd[1] != '\0') {
        int idx   = cmd[0] - '1';
        int angle = 0;
        for (int i = 1; cmd[i] != '\0'; i++)
            angle = angle * 10 + (cmd[i] - '0');
        float safe = Clamp(idx, (float)angle);
        Servo_SetTarget(idx, safe);
        xil_printf("[CMD] Servo%d -> %d deg\r\n", idx + 1, (int)safe);
        return;
    }

    xil_printf("[CMD] Unknown: %s\r\n", cmd);
}

/* ============================================================
 * main
 * ============================================================ */
int main(void)
{
    init_platform();

    xil_printf("\r\n============================================\r\n");
    xil_printf(" Vision Robot Arm - State Machine Mode\r\n");
    xil_printf("============================================\r\n");

    Servo_Init();
    SM_Init();
    // Print_Help();

    char rx_buf[UART_BUF_SIZE];
    int  rx_idx = 0;

    while (1) {
        /* ── UART 수신 (논블로킹) ── */
        while (!XUartLite_IsReceiveEmpty(UART_BASE)) {
            u8 ch = XUartLite_RecvByte(UART_BASE);

            if (ch == '\r' || ch == '\n') {
                rx_buf[rx_idx] = '\0';
                if (rx_idx > 0) {
                    xil_printf("\r\n[INPUT] %s\r\n", rx_buf);
                    Process_Command(rx_buf);
                }
                rx_idx = 0;
            } else if (ch == 8u || ch == 127u) {
                if (rx_idx > 0) rx_idx--;
            } else {
                if (rx_idx < (int)UART_BUF_SIZE - 1) {
                    rx_buf[rx_idx++] = (char)ch;
                } else {
                    rx_idx = 0;
                    xil_printf("\r\n[ERROR] Command too long\r\n");
                }
            }
        }

        /* ── State Machine 업데이트 ── */
        SM_Update();

        usleep(MAIN_LOOP_US);
    }

    cleanup_platform();
    return 0;
}