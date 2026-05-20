#ifndef ROBOT_CONFIG_H
#define ROBOT_CONFIG_H

#include "xparameters.h"
#include "xil_types.h"

/* ============================================================
 * 하드웨어 베이스 주소
 * ============================================================ */
#define CAM_SHAPE_BASE    XPAR_MYIP_SHAPE_CLASSIFIER_0_BASEADDR
#define PWM_BASE          XPAR_MY_PWM_6CH_0_BASEADDR
#define UART_BASE         XPAR_AXI_UARTLITE_0_BASEADDR

/* ============================================================
 * 서보 인덱스
 * ============================================================ */
#define SERVO_COUNT     6
#define SERVO_BASE_H    0   /* 베이스 가로 축 */
#define SERVO_BASE_V    1   /* 베이스 세로 축 */
#define SERVO_ELBOW     2   /* 팔꿈치        */
#define SERVO_WRIST_V   3   /* 손목 세로 축  */
#define SERVO_WRIST_H   4   /* 손목 가로 축  */
#define SERVO_GRIPPER   5   /* 집게          */

/* ============================================================
 * 집게 안전 각도
 * ============================================================ */
#define GRIPPER_MIN_ANGLE   90.0f
#define GRIPPER_GRAB_ANGLE  127.0f
#define GRIPPER_MAX_ANGLE   170.0f

/* ============================================================
 * PWM 파라미터
 * ============================================================ */
#define BASE_DUTY_OFFSET    10000u
#define PWM_CENTER_DUTY     150000u

/* ============================================================
 * 이동 속도 (duty step / loop)
 * ============================================================ */
#define STEP_SMOOTH         400u
#define STEP_AUTO           250u
#define STEP_PLACE_LOWER    120u
#define STEP_HOME           350u

/* ============================================================
 * 타이밍
 * ============================================================ */
#define MAIN_LOOP_US        10000u          /* 메인 루프 주기 */
#define CAM_POLL_INTERVAL   100u            /* 루프 * MAIN_LOOP_US = 1s */
#define TIME_WAIT_LOOPS(ms) ((ms) * 1000u / MAIN_LOOP_US)

/* ============================================================
 * 그리드
 * ============================================================ */
#define GRID_SIZE           3

/* ============================================================
 * 집기/놓기 델타 각도
 * ============================================================ */
#define PICK_ELBOW_DELTA     13.0f
#define PICK_WRIST_DELTA    -17.0f
#define PLACE_ELBOW_DELTA    10.0f
#define PLACE_WRIST_DELTA   -10.0f

/* ============================================================
 * UART 버퍼
 * ============================================================ */
#define UART_BUF_SIZE       32u

/* ============================================================
 * CAM 도형 코드
 * ============================================================ */
#define CAM_NONE_WHITE  0x0u   /* 4'b0000 : 흰 배경  */
#define CAM_NONE_BLACK  0xFu   /* 4'b1111 : 검은 배경 */
#define CAM_UNKNOWN     0xAu   /* 4'b1010 : 미인식    */
#define CAM_CIRCLE      0x1u   /* 4'b0001 : ○         */
#define CAM_RECT        0x7u   /* 4'b0111 : □         */
#define CAM_CROSS       0xEu   /* 4'b1110 : +×        */

/* 도형별 픽업 접근 위치 */
#define PICK_CIRCLE_ROW  0
#define PICK_CIRCLE_COL  0
#define PICK_RECT_ROW    0
#define PICK_RECT_COL    1
#define PICK_CROSS_ROW   0
#define PICK_CROSS_COL   2

#endif /* ROBOT_CONFIG_H */