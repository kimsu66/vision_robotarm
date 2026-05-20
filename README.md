# OV7670 Shape Classifier — Basys-3 FPGA

OV7670 카메라 영상에서 실시간으로 도형(○ / □ / +×)을 인식하고 LED로 출력하는 FPGA 시스템.  
Verilog RTL로 전체 파이프라인을 구현하였으며, 외부 MCU나 소프트코어 없이 순수 하드웨어 로직으로 동작한다.

---

## System Overview

```
OV7670
  └─→ ov7670_capture  (PCLK 동기 pixel 수집)
        └─→ frame_buffer  (BRAM, RGB565)
              └─→ box_sampler  (56×56 → 28×28 다운샘플)
                    └─→ shape_classifier  (3×3 grid 잉크 분포 분석)
                          └─→ LED 출력  (4-bit 인코딩)
```

VGA 출력 구성:
- 좌(0~319px): 카메라 원본 + 빨간(140×140) / 파란(56×56) 박스 오버레이
- 구분선(326~329px): 흰색 세로선
- 우(340~619px): 28×28 스냅샷 × 10배 업스케일 표시

---

## Shape Classification Logic

입력 프레임을 3×3 grid로 분할하고, 셀별 잉크량(픽셀 밀도)을 기반으로 도형을 판별한다.

```
[g00][g01][g02]
[g10][g11][g12]    g11 = 중앙 셀 (10×10px, max 100)
[g20][g21][g22]
```

| 도형 | 판정 조건 |
|------|-----------|
| ○ (원) | g11 ≥ MIN_CIRCLE_G11 AND g11 > surround (중앙 > 주변 합) |
| □ (사각형) | g11 ≤ MAX_HOLLOW AND corner ≥ MIN_SQUARE_CORNER |
| +× (십자/대각) | g11 ≥ MIN_CENTER_INK AND (plus_arms ≫ corner 또는 반대) |
| NONE | total < MIN_INK (흰 배경) 또는 total > MAX_INK (검은 배경) |

4프레임 연속 동일 결과 시 LED 갱신 (노이즈 방지).

**LED 인코딩:**

| LED | 의미 |
|-----|------|
| `4'b0001` | ○ (원) |
| `4'b0111` | □ (사각형) |
| `4'b1110` | +× (십자/대각) |
| `4'b0000` | NONE (흰 배경) |
| `4'b1111` | 검은 배경 |
| `4'b1010` | UNKNOWN |

---

## Source Structure

```
sources_1/
├── imports/new/
│   ├── camera.v            # OV7670 SCCB 초기화 + 클럭 생성
│   ├── ov7670_capture.v    # PCLK 동기 pixel 캡처
│   ├── ov7670_init.v       # 레지스터 초기화 시퀀스
│   ├── frame_buffer.v      # BRAM 기반 프레임 버퍼 (RGB565)
│   └── vga_controller.v   # VGA 타이밍 생성
└── new/
    ├── box_sampler.v        # 56×56 → 28×28 다운샘플러
    ├── shape_classifier.v   # 도형 분류 (3×3 grid 잉크 분석)
    ├── digit_detector.v     # (실험용)
    ├── top_camera_shape.v   # 최종 탑 모듈 (카메라 + 분류 + VGA)
    ├── uart_rx.v / uart_tx.v
    └── ...

sim_1/new/
    ├── uart_rx_tb.v / uart_tx_tb.v
    └── ...
```

> MNIST 관련 소스(`fc1_layer`, `fc2_layer`, `argmax` 등)는 실험 브랜치에 존재하며, 이 브랜치에서는 미사용.

---

## Target Board

| 항목 | 내용 |
|------|------|
| Board | Digilent Basys-3 (Artix-7 XC7A35T) |
| Tool | Vivado 2024.x |
| Camera | OV7670 (RGB565 모드) |
| 출력 | VGA 640×480 + LED 4bit |

---

## Branch Structure

| Branch | 내용 |
|--------|------|
| `main` | OV7670 카메라 + 도형 분류 (이 브랜치) |
| `robot-arm` | 도형 분류 결과 → 로봇 암 PWM 제어 (IP화) |

---

## Notes

- OV7670은 RGB565 모드로 설정 (`COM15` 레지스터). YUV 모드와 byte 순서 주의.
- `box_sampler`는 캡처 영역 중앙 56×56을 추출한 뒤 2×2 평균으로 28×28 압축.
- `shape_classifier` 파라미터(`MIN_INK`, `MIN_CIRCLE_G11` 등)는 조명 환경에 따라 튜닝 필요.
