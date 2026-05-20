#include "servo.h"
#include "robot_config.h"
#include "xil_io.h"

/* ============================================================
 * 홈/코너 각도 테이블
 * ============================================================ */
static const float s_home[SERVO_COUNT] = {
    // 90.0f, 90.0f, 140.0f, 150.0f, 90.0f, 90.0f
    0.0f, 90.0f, 115.0f, 170.0f, 90.0f, 90.0f
};

// home {90, 90, 140, 150, 90, 90} 기준 원본값
static const float s_near_l[SERVO_COUNT] = { 63.5f, 104.4f, 114.3f, 143.5f, 90.0f, 90.0f };
static const float s_near_r[SERVO_COUNT] = { 112.4f, 104.4f, 115.1f, 143.5f, 90.0f, 90.0f };
static const float s_far_l[SERVO_COUNT]  = { 55.5f, 116.4f, 95.1f, 180.0f, 90.0f, 90.0f };
static const float s_far_r[SERVO_COUNT]  = { 125.2f, 116.4f, 95.1f, 180.0f, 90.0f, 90.0f };

// // home {0, 90, 115, 170, 90, 90} 기준: ELBOW-25, WRIST_V+20 적용
// static const float s_near_l[SERVO_COUNT] = {
//     63.5f, 104.4f, 114.3f, 163.5f, 90.0f, 90.0f
// };
// static const float s_near_r[SERVO_COUNT] = {
//     112.4f, 104.4f, 115.1f, 163.5f, 90.0f, 90.0f
// };
// static const float s_far_l[SERVO_COUNT] = {
//     55.5f, 116.4f, 95.1f, 180.0f, 90.0f, 90.0f
// };
// static const float s_far_r[SERVO_COUNT] = {
//     125.2f, 116.4f, 95.1f, 180.0f, 90.0f, 90.0f
// };

/* ============================================================
 * 내부 상태
 * ============================================================ */
static float s_target[SERVO_COUNT];
static u32   s_duty[SERVO_COUNT];

/* ============================================================
 * 내부 유틸
 * ============================================================ */
float Servo_ClampAngle(int idx, float deg)
{
    if (deg < 0.0f)   deg = 0.0f;
    if (deg > 180.0f) deg = 180.0f;
    if (idx == SERVO_GRIPPER) {
        if (deg < GRIPPER_MIN_ANGLE)  deg = GRIPPER_MIN_ANGLE;
        if (deg > GRIPPER_MAX_ANGLE)  deg = GRIPPER_MAX_ANGLE;
    }
    return deg;
}

static u32 Calc_Duty(int idx, float deg)
{
    deg = Servo_ClampAngle(idx, deg);
    u32 duty = (u32)(150000.0f + (deg - 90.0f) * 1111.1f);
    if (idx == SERVO_BASE_H) duty += BASE_DUTY_OFFSET;
    return duty;
}

/* ============================================================
 * 공개 함수 구현
 * ============================================================ */
void Servo_Init(void)
{
    for (int i = 0; i < SERVO_COUNT; i++) {
        s_target[i] = s_home[i];
        s_duty[i]   = Calc_Duty(i, s_target[i]);
        Xil_Out32(PWM_BASE + (u32)(i * 4), s_duty[i]);
    }
}

void Servo_SetTarget(int idx, float deg)
{
    s_target[idx] = Servo_ClampAngle(idx, deg);
}

void Servo_AddTarget(int idx, float delta)
{
    Servo_SetTarget(idx, s_target[idx] + delta);
}

void Servo_SetAllHome(void)
{
    for (int i = 0; i < SERVO_COUNT; i++)
        Servo_SetTarget(i, s_home[i]);
}

float Servo_GetTarget(int idx)
{
    return s_target[idx];
}

int Servo_Update(u32 step_limit)
{
    int all_arrived = 1;

    for (int i = 0; i < SERVO_COUNT; i++) {
        u32 tgt = Calc_Duty(i, s_target[i]);
        if (s_duty[i] == tgt) continue;

        all_arrived = 0;
        u32 diff = (s_duty[i] < tgt) ? tgt - s_duty[i]
                                      : s_duty[i] - tgt;
        u32 step = (diff > step_limit) ? step_limit : diff;

        if (s_duty[i] < tgt) s_duty[i] += step;
        else                  s_duty[i] -= step;

        Xil_Out32(PWM_BASE + (u32)(i * 4), s_duty[i]);
    }

    return all_arrived;
}

void Servo_Freeze(void)
{
    for (int i = 0; i < SERVO_COUNT; i++) {
        float duty = (float)s_duty[i];
        float deg;
        if (i == SERVO_BASE_H)
            deg = (duty - (float)BASE_DUTY_OFFSET - 150000.0f) / 1111.1f + 90.0f;
        else
            deg = (duty - 150000.0f) / 1111.1f + 90.0f;
        s_target[i] = Servo_ClampAngle(i, deg);
    }
}

void Servo_SetTargetByGrid(int row, int col)
{
    float v = (float)row / (float)(GRID_SIZE - 1);
    float u = (float)col / (float)(GRID_SIZE - 1);

    for (int i = 0; i < SERVO_COUNT; i++) {
        if (i == SERVO_GRIPPER) continue; // ksy 추가
        if (i == SERVO_BASE_V) continue;
        float deg =
            (1.0f - u) * (1.0f - v) * s_near_l[i] +
            u           * (1.0f - v) * s_near_r[i] +
            (1.0f - u) * v           * s_far_l[i]  +
            u           * v           * s_far_r[i];
        Servo_SetTarget(i, deg);
    }
}