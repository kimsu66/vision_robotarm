#include "state_machine.h"
#include "robot_config.h"
#include "servo.h"
#include "cam_shape.h"
#include "xil_printf.h"

/* ============================================================
 * 내부 상태 변수
 * ============================================================ */
static RobotState s_state       = STATE_IDLE;
static ActStep    s_act_step    = FIRST;
static int        s_place_row   = 0;
static int        s_place_col   = 0;

static u32 s_cam_cnt  = 0u;  /* CAM 폴링 카운터  */
static u32 s_wait_cnt = 0u;  /* 시간 대기 카운터 */

// state_machine.c 내부
static int s_pick_row;
static int s_pick_col;

/* ============================================================
 * 내부 유틸
 * ============================================================ */
static void Transition_To(RobotState next)
{
    s_state = next;
    // const char *name[] = { "IDLE", "READY", "ACT", "HOME", "STOP" };
    const char *name[] = { "IDLE", "ACT", "HOME", "STOP" };
    xil_printf("\r\n>>> State : %s\r\n\r\n", name[(int)next]);
}

/* 논블로킹 시간 대기: loops 루프 경과 시 1 반환 */
static int Wait_Time(u32 loops)
{
    Servo_Update(STEP_AUTO);
    s_wait_cnt++;
    if (s_wait_cnt >= loops) {
        s_wait_cnt = 0u;
        return 1;
    }
    return 0;
}

/* ============================================================
 * 상태별 핸들러
 * ============================================================ */
static void Handle_Idle(void)
{
    Servo_Update(STEP_SMOOTH);

    s_cam_cnt++;
    if (s_cam_cnt < CAM_POLL_INTERVAL) return;
    s_cam_cnt = 0u;

    int shape = CamShape_Read();
    if (!CamShape_IsValid(shape)) return;

    CamShape_ToGrid(shape, &s_place_row, &s_place_col);
    CamShape_ToPickPos(shape, &s_pick_row, &s_pick_col);

    const char *shape_name =
        (shape == (int)CAM_CIRCLE) ? "Circle (O)"  :
        (shape == (int)CAM_RECT)   ? "Square   ([])" :
        (shape == (int)CAM_CROSS)  ? "Cross  (+x)" : "Unknown";

    xil_printf("\r\n");
    xil_printf("========================================\r\n");
    xil_printf("  Shape : %s\r\n", shape_name);
    xil_printf("  Pick  : Grid[%d,%d]\r\n", s_pick_row, s_pick_col);
    xil_printf("  Place : Grid[%d,%d]\r\n", s_place_row, s_place_col);
    xil_printf("========================================\r\n");
    // xil_printf("  Confirm? [Y] proceed / [N] cancel\r\n");  /* 추가 */


    s_act_step  = FIRST;
    s_wait_cnt  = 0u;
    // Transition_To(STATE_READY);
    Transition_To(STATE_ACT);
}

/* Handle_Ready 추가 */
static void Handle_Ready(void)
{
    Servo_Update(STEP_SMOOTH);
}

// #define SERVO_COUNT     6
// #define SERVO_BASE_H    1   /* 베이스 가로 축 */
// #define SERVO_BASE_V    2   /* 베이스 세로 축 */
// #define SERVO_ELBOW     3   /* 팔꿈치        */
// #define SERVO_WRIST_V   4   /* 손목 세로 축  */
// #define SERVO_WRIST_H   5   /* 손목 가로 축  */
// #define SERVO_GRIPPER   6   /* 집게          */

static void Handle_Act(void)
{
    switch (s_act_step) {
    case FIRST:
        xil_printf("ACT 1. Go back\n");
        Servo_SetTarget(SERVO_BASE_V, 95.0f);
        Servo_SetTarget(SERVO_WRIST_V, 150.0f);
        s_act_step = SECOND;
        break;

    case SECOND: 
        if (!Servo_Update(STEP_AUTO)) break;
        xil_printf("ACT 2. Grabbing\r\n");
        Servo_SetTarget(SERVO_GRIPPER, 125.0f);
        s_act_step = THIRD;
        break;

    case THIRD: 
        if (!Servo_Update(STEP_AUTO)) break;
        xil_printf("ACT 3. Go Up \r\n");
        Servo_SetTarget(SERVO_BASE_V, 80.0f);
        Servo_SetTarget(SERVO_ELBOW, 110.0f);
        s_act_step = FORTH;
        break;

    case FORTH:
        if (!Servo_Update(STEP_AUTO)) break;
        xil_printf("ACT 4. Go to Grid[%d,%d]\r\n", s_pick_row, s_pick_col);
        Servo_SetTargetByGrid(s_pick_row, s_pick_col);
        s_act_step = FIFTH;
        break;

    case FIFTH:
        if (!Servo_Update(STEP_AUTO)) break;
        xil_printf("ACT 5. Go Down\r\n");
        Servo_SetTarget(SERVO_ELBOW, 145.0f);
        s_act_step = FIFTH_B;
        break;

    case FIFTH_B:
        if (!Servo_Update(STEP_AUTO)) break;
        if (!Wait_Time(50)) break;
        xil_printf("ACT Waiting.. \r\n");
        Servo_SetTarget(SERVO_GRIPPER, 90.0f);
        s_act_step = SIXTH;
        break;

    case SIXTH:
        if (!Servo_Update(STEP_AUTO)) break;
        if(!(Wait_Time(50))) break;
        xil_printf("ACT 6. go back to IDLE and wait\r\n");
        
        Servo_SetTarget(SERVO_ELBOW, 115.0f);

        s_act_step = SEVENTH;
        break;

    case SEVENTH:
        if (!Servo_Update(STEP_AUTO)) break;
        if(!(Wait_Time(50))) break;
        xil_printf("ACT 7. go back to IDLE 2\r\n");

        Servo_SetTarget(SERVO_BASE_H, 0.0f);
        Servo_SetTarget(SERVO_BASE_V, 90.0f);
        Servo_SetTarget(SERVO_WRIST_V, 170.0f);
        Servo_SetTarget(SERVO_WRIST_H, 90.0f);
        Transition_To(STATE_HOME);
    }



}

static void Handle_Home(void)
{
    if (Servo_Update(STEP_HOME)) {
        xil_printf("[HOME] Arrived -> IDLE\r\n");
        Transition_To(STATE_IDLE);
    }
}

static void Handle_Stop(void)
{
    /* 서보 현재 위치 유지, UART 명령 대기 */
}

/* ============================================================
 * 공개 함수
 * ============================================================ */
void SM_Init(void)
{
    s_state    = STATE_IDLE;
    s_act_step = FIRST;
    s_cam_cnt  = 0u;
    s_wait_cnt = 0u;
    xil_printf("[STATE] -> IDLE\r\n");
}

void SM_Update(void)
{
    switch (s_state) {
        case STATE_IDLE: Handle_Idle(); break;
        // case STATE_READY: Handle_Ready(); break;
        case STATE_ACT:  Handle_Act();  break;
        case STATE_HOME: Handle_Home(); break;
        case STATE_STOP: Handle_Stop(); break;
    }
}

/* 추가 함수 */
void SM_CmdConfirm(void)
{
    xil_printf("[CMD] Y: READY state not used\r\n");
}


void SM_CmdCancel(void)
{
    xil_printf("[CMD] Cancelled -> IDLE\r\n");
    Transition_To(STATE_IDLE);
}

void SM_CmdHome(void)
{
    xil_printf("[CMD] HOME (force)\r\n");
    Servo_SetAllHome();
    s_act_step = FIRST;
    s_wait_cnt = 0u;
    Transition_To(STATE_HOME);
}

void SM_CmdStop(void)
{
    xil_printf("[CMD] STOP\r\n");
    Servo_Freeze();
    Transition_To(STATE_STOP);
}

void SM_CmdResume(void)
{
    if (s_state == STATE_STOP) {
        xil_printf("[CMD] RESUME -> IDLE\r\n");
        Transition_To(STATE_IDLE);
    } else {
        xil_printf("[CMD] R: not in STOP state\r\n");
    }
}

void SM_CmdForceAct(int pick_row, int pick_col)
{
    if (SM_GetState() != STATE_IDLE && SM_GetState() != STATE_STOP) {
        xil_printf("[CMD] A: not in IDLE/STOP state\r\n");
        return;
    }
    xil_printf("[DEBUG] Force ACT -> Pick[%d,%d]\r\n", pick_row, pick_col);
    s_pick_row  = pick_row;
    s_pick_col  = pick_col;
    s_act_step  = FIRST;
    s_wait_cnt  = 0u;
    Transition_To(STATE_ACT);
}

void SM_CmdManualGrid(int row, int col)
{
    xil_printf("[CMD] Manual Grid -> [%d,%d]\r\n", row, col);
    s_place_row = row;
    s_place_col = col;
    s_act_step  = FIRST;
    s_wait_cnt  = 0u;
    Transition_To(STATE_ACT);
}

RobotState SM_GetState(void)
{
    return s_state;
}