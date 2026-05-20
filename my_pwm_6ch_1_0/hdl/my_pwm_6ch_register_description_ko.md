# my_pwm_6ch IP Register Description (Korean)

## 1. Register Summary

`my_pwm_6ch` IP는 AXI4-Lite 슬레이브 인터페이스를 통해 접근되는 8개의 32-bit 레지스터를 제공한다. 모든 레지스터는 4바이트 정렬이며 reset 시 `0x00000000`으로 초기화된다.

| Offset | Register | Access | Initial Value | Description |
| --- | --- | --- | --- | --- |
| `0x00` | `SLV_REG0 (PWM_DUTY1)` | `R/W` | `0x00000000` | `servo1` PWM high 구간 카운트 값 |
| `0x04` | `SLV_REG1 (PWM_DUTY2)` | `R/W` | `0x00000000` | `servo2` PWM high 구간 카운트 값 |
| `0x08` | `SLV_REG2 (PWM_DUTY3)` | `R/W` | `0x00000000` | `servo3` PWM high 구간 카운트 값 |
| `0x0C` | `SLV_REG3 (PWM_DUTY4)` | `R/W` | `0x00000000` | `servo4` PWM high 구간 카운트 값 |
| `0x10` | `SLV_REG4 (PWM_DUTY5)` | `R/W` | `0x00000000` | `servo5` PWM high 구간 카운트 값 |
| `0x14` | `SLV_REG5 (PWM_DUTY6)` | `R/W` | `0x00000000` | `servo6` PWM high 구간 카운트 값 |
| `0x18` | `SLV_REG6 (RESERVED0)` | `R/W` | `0x00000000` | 현재 사용자 로직에 연결되지 않은 예약 레지스터 |
| `0x1C` | `SLV_REG7 (RESERVED1)` | `R/W` | `0x00000000` | 현재 사용자 로직에 연결되지 않은 예약 레지스터 |

## 2. Register Description

### 2.1 `SLV_REG0` - PWM Duty Register 1

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY1[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG0`는 `servo1` 출력의 PWM high 구간 길이를 제어한다. 내부 PWM 로직은 `counter < DUTY1`인 동안 `servo1`을 High로 구동한다.

- Bits `31:0` - `DUTY1`: PWM duty count value
  이 필드에 기록한 값은 한 PWM 프레임 내에서 `servo1`이 High를 유지하는 카운트 수를 정의한다. 구현 기준으로 `PERIOD = 2,000,000`이며, 소스 주석 기준 `S_AXI_ACLK = 100 MHz`일 때 약 20 ms 프레임에 해당한다. 같은 가정에서 1카운트는 10 ns이며, 1.0 ms는 100,000카운트, 1.5 ms는 150,000카운트, 2.0 ms는 200,000카운트에 해당한다.

### 2.2 `SLV_REG1` - PWM Duty Register 2

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY2[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG1`은 `servo2` 출력의 PWM high 구간 길이를 제어한다. 내부 PWM 로직은 `counter < DUTY2`인 동안 `servo2`를 High로 구동한다.

- Bits `31:0` - `DUTY2`: PWM duty count value
  이 필드에 기록한 값은 한 PWM 프레임 내에서 `servo2`가 High를 유지하는 카운트 수를 정의한다. 카운트 단위와 시간 환산은 `SLV_REG0`와 동일하다.

### 2.3 `SLV_REG2` - PWM Duty Register 3

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY3[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG2`는 `servo3` 출력의 PWM high 구간 길이를 제어한다. 내부 PWM 로직은 `counter < DUTY3`인 동안 `servo3`를 High로 구동한다.

- Bits `31:0` - `DUTY3`: PWM duty count value
  이 필드에 기록한 값은 한 PWM 프레임 내에서 `servo3`가 High를 유지하는 카운트 수를 정의한다. 카운트 단위와 시간 환산은 `SLV_REG0`와 동일하다.

### 2.4 `SLV_REG3` - PWM Duty Register 4

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY4[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG3`는 `servo4` 출력의 PWM high 구간 길이를 제어한다. 내부 PWM 로직은 `counter < DUTY4`인 동안 `servo4`를 High로 구동한다.

- Bits `31:0` - `DUTY4`: PWM duty count value
  이 필드에 기록한 값은 한 PWM 프레임 내에서 `servo4`가 High를 유지하는 카운트 수를 정의한다. 카운트 단위와 시간 환산은 `SLV_REG0`와 동일하다.

### 2.5 `SLV_REG4` - PWM Duty Register 5

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY5[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG4`는 `servo5` 출력의 PWM high 구간 길이를 제어한다. 내부 PWM 로직은 `counter < DUTY5`인 동안 `servo5`를 High로 구동한다.

- Bits `31:0` - `DUTY5`: PWM duty count value
  이 필드에 기록한 값은 한 PWM 프레임 내에서 `servo5`가 High를 유지하는 카운트 수를 정의한다. 카운트 단위와 시간 환산은 `SLV_REG0`와 동일하다.

### 2.6 `SLV_REG5` - PWM Duty Register 6

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY6[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG5`는 `servo6` 출력의 PWM high 구간 길이를 제어한다. 내부 PWM 로직은 `counter < DUTY6`인 동안 `servo6`를 High로 구동한다.

- Bits `31:0` - `DUTY6`: PWM duty count value
  이 필드에 기록한 값은 한 PWM 프레임 내에서 `servo6`가 High를 유지하는 카운트 수를 정의한다. 카운트 단위와 시간 환산은 `SLV_REG0`와 동일하다.

### 2.7 `SLV_REG6` - Reserved Register 0

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `RESERVED0[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG6`는 AXI 주소 공간에는 존재하지만 현재 `pwm_servo_6ch` 사용자 로직에는 연결되어 있지 않다.

- Bits `31:0` - `RESERVED0`: Reserved field
  현재 구현에서는 이 필드가 PWM 출력이나 다른 내부 제어 신호에 영향을 주지 않는다. 향후 사용자 로직 확장을 고려하면 소프트웨어는 특별한 목적이 없는 한 `0x00000000`을 기록하는 것이 바람직하다.

### 2.8 `SLV_REG7` - Reserved Register 1

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `RESERVED1[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG7`는 AXI 주소 공간에는 존재하지만 현재 `pwm_servo_6ch` 사용자 로직에는 연결되어 있지 않다.

- Bits `31:0` - `RESERVED1`: Reserved field
  현재 구현에서는 이 필드가 PWM 출력이나 다른 내부 제어 신호에 영향을 주지 않는다. 향후 사용자 로직 확장을 고려하면 소프트웨어는 특별한 목적이 없는 한 `0x00000000`을 기록하는 것이 바람직하다.

## 3. Operation Notes

1. `SLV_REG0`부터 `SLV_REG5`까지는 각각 `servo1`부터 `servo6`까지 일대일로 대응된다.
2. 모든 레지스터는 소프트웨어 read-back이 가능하며, read 시 마지막으로 기록된 값이 반환된다.
3. reset(`S_AXI_ARESETN = 0`) 시 모든 레지스터는 `0`으로 초기화되므로 모든 PWM 출력은 비활성 상태로 시작한다.
4. `DUTYx` 값이 `PERIOD`보다 크면 내부 카운터가 그 값까지 도달하지 못하므로 출력이 거의 전체 프레임 동안 High 상태를 유지할 수 있다.
5. 시간 환산 예시는 소스 코드의 `PERIOD = 2,000,000`과 주석의 `20 ms @100 MHz`를 기준으로 정리했다. 실제 시스템 클럭이 다르면 펄스 폭도 비례하여 달라진다.
