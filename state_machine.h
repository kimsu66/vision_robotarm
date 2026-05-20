// States:
//   IDLE  : 카메라 폴링, UART 수신 대기
//   ACT   : 도형 동작 실행 중 (카메라 입력 무시)
//   HOME  : 홈 복귀 중 (H 입력 또는 ACT 완료 후)
//   STOP  : 정지 (S 입력, 서보 현재 위치 유지)

// 전이:
//   IDLE → ACT   : 카메라에서 유효 도형 감지
//   IDLE → STOP  : S 입력
//   ACT  → HOME  : 동작 완료 (Place_At_Grid 끝)
//   ACT  → HOME  : H 입력 (인터럽트성, 즉시 전이)
//   HOME → IDLE  : 홈 복귀 완료
//   STOP → IDLE  : H 입력
//   STOP → IDLE  : R 입력 (resume)
//   any  → HOME  : H 입력

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "robot_config.h"

/* ============================================================
 * 로봇 최상위 상태
 * ============================================================ */
typedef enum {
    STATE_IDLE = 0,  /* 카메라 폴링, UART 대기 */
    
    STATE_ACT,       /* 도형 동작 실행 중       */
    STATE_HOME,      /* 홈 복귀 중              */
    STATE_STOP       /* 정지 (현재 위치 유지)   */
} RobotState;

/* ============================================================
 * ACT 내부 단계
 * ============================================================ */
typedef enum {
    FIRST = 0,
    SECOND,           /* 도달 대기 → 집게 닫기              */
    THIRD,
    FORTH,
    FIFTH,
    FIFTH_B,
    SIXTH,
    SEVENTH
} ActStep;

/* ============================================================
 * 초기화
 * ============================================================ */
void SM_Init(void);

/* ============================================================
 * 매 루프 호출: 상태 업데이트
 * ============================================================ */
void SM_Update(void);

/* ============================================================
 * 외부 명령 (UART 핸들러에서 호출)
 * ============================================================ */
void SM_CmdHome(void);   /* H: 즉시 홈 복귀         */
void SM_CmdStop(void);   /* S: 정지                  */
void SM_CmdResume(void); /* R: 정지 해제 → IDLE      */
void SM_CmdManualGrid(int row, int col); /* 수동 그리드 */
void SM_CmdConfirm(void);  /* Y: ACT 진행 */
void SM_CmdCancel(void);   /* N: IDLE 복귀 */
void SM_CmdForceAct(int pick_row, int pick_col); /* A: 디버그용 강제 ACT */


/* ============================================================
 * 상태 조회
 * ============================================================ */
RobotState SM_GetState(void);

#endif /* STATE_MACHINE_H */