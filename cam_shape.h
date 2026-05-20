#ifndef CAM_SHAPE_H
#define CAM_SHAPE_H

#include "robot_config.h"

/* ============================================================
 * 도형 읽기
 * 반환: CAM_CIRCLE / CAM_RECT / CAM_CROSS → 유효 도형
 *       -1                                → 정지 (NONE/UNKNOWN)
 * ============================================================ */
int  CamShape_Read(void);

/* ============================================================
 * 유효 도형 여부
 * ============================================================ */
int  CamShape_IsValid(int shape_code);

/* ============================================================
 * 도형 코드 → 배치 그리드 (row, col)
 * ============================================================ */
void CamShape_ToGrid(int shape_code, int *row, int *col);

void CamShape_ToPickPos(int shape_code, int *row, int *col);

#endif /* CAM_SHAPE_H */