# my_pwm_6ch IP Register Description (English)

## 1. Register Summary

The `my_pwm_6ch` IP provides eight 32-bit registers through an AXI4-Lite slave interface. All registers are 4-byte aligned and reset to `0x00000000`.

| Offset | Register | Access | Initial Value | Description |
| --- | --- | --- | --- | --- |
| `0x00` | `SLV_REG0 (PWM_DUTY1)` | `R/W` | `0x00000000` | PWM high-time count for `servo1` |
| `0x04` | `SLV_REG1 (PWM_DUTY2)` | `R/W` | `0x00000000` | PWM high-time count for `servo2` |
| `0x08` | `SLV_REG2 (PWM_DUTY3)` | `R/W` | `0x00000000` | PWM high-time count for `servo3` |
| `0x0C` | `SLV_REG3 (PWM_DUTY4)` | `R/W` | `0x00000000` | PWM high-time count for `servo4` |
| `0x10` | `SLV_REG4 (PWM_DUTY5)` | `R/W` | `0x00000000` | PWM high-time count for `servo5` |
| `0x14` | `SLV_REG5 (PWM_DUTY6)` | `R/W` | `0x00000000` | PWM high-time count for `servo6` |
| `0x18` | `SLV_REG6 (RESERVED0)` | `R/W` | `0x00000000` | Reserved register not connected to current user logic |
| `0x1C` | `SLV_REG7 (RESERVED1)` | `R/W` | `0x00000000` | Reserved register not connected to current user logic |

## 2. Register Description

### 2.1 `SLV_REG0` - PWM Duty Register 1

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY1[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG0` controls the PWM high interval of the `servo1` output. The internal PWM logic drives `servo1` high while `counter < DUTY1`.

- Bits `31:0` - `DUTY1`: PWM duty count value
  The value written to this field defines the number of counts for which `servo1` remains high within one PWM frame. In the current implementation, `PERIOD = 2,000,000`, which corresponds to an approximately 20 ms frame when `S_AXI_ACLK = 100 MHz`, as indicated by the source comment. Under the same assumption, one count equals 10 ns, and typical servo pulse widths are 100,000 counts for 1.0 ms, 150,000 counts for 1.5 ms, and 200,000 counts for 2.0 ms.

### 2.2 `SLV_REG1` - PWM Duty Register 2

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY2[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG1` controls the PWM high interval of the `servo2` output. The internal PWM logic drives `servo2` high while `counter < DUTY2`.

- Bits `31:0` - `DUTY2`: PWM duty count value
  The value written to this field defines the number of counts for which `servo2` remains high within one PWM frame. The count unit and time conversion are identical to `SLV_REG0`.

### 2.3 `SLV_REG2` - PWM Duty Register 3

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY3[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG2` controls the PWM high interval of the `servo3` output. The internal PWM logic drives `servo3` high while `counter < DUTY3`.

- Bits `31:0` - `DUTY3`: PWM duty count value
  The value written to this field defines the number of counts for which `servo3` remains high within one PWM frame. The count unit and time conversion are identical to `SLV_REG0`.

### 2.4 `SLV_REG3` - PWM Duty Register 4

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY4[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG3` controls the PWM high interval of the `servo4` output. The internal PWM logic drives `servo4` high while `counter < DUTY4`.

- Bits `31:0` - `DUTY4`: PWM duty count value
  The value written to this field defines the number of counts for which `servo4` remains high within one PWM frame. The count unit and time conversion are identical to `SLV_REG0`.

### 2.5 `SLV_REG4` - PWM Duty Register 5

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY5[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG4` controls the PWM high interval of the `servo5` output. The internal PWM logic drives `servo5` high while `counter < DUTY5`.

- Bits `31:0` - `DUTY5`: PWM duty count value
  The value written to this field defines the number of counts for which `servo5` remains high within one PWM frame. The count unit and time conversion are identical to `SLV_REG0`.

### 2.6 `SLV_REG5` - PWM Duty Register 6

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `DUTY6[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG5` controls the PWM high interval of the `servo6` output. The internal PWM logic drives `servo6` high while `counter < DUTY6`.

- Bits `31:0` - `DUTY6`: PWM duty count value
  The value written to this field defines the number of counts for which `servo6` remains high within one PWM frame. The count unit and time conversion are identical to `SLV_REG0`.

### 2.7 `SLV_REG6` - Reserved Register 0

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `RESERVED0[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG6` exists in the AXI address space but is not connected to the current `pwm_servo_6ch` user logic.

- Bits `31:0` - `RESERVED0`: Reserved field
  In the current implementation, this field does not affect any PWM output or internal control signal. For forward compatibility, software should write `0x00000000` unless a future user extension defines another purpose.

### 2.8 `SLV_REG7` - Reserved Register 1

| Item | Value |
| --- | --- |
| Bit | `31:0` |
| Field | `RESERVED1[31:0]` |
| Read/Write | `R/W` |
| Initial Value | `0x00000000` |

`SLV_REG7` exists in the AXI address space but is not connected to the current `pwm_servo_6ch` user logic.

- Bits `31:0` - `RESERVED1`: Reserved field
  In the current implementation, this field does not affect any PWM output or internal control signal. For forward compatibility, software should write `0x00000000` unless a future user extension defines another purpose.

## 3. Operation Notes

1. `SLV_REG0` through `SLV_REG5` map one-to-one to `servo1` through `servo6`.
2. All registers support software read-back, and a read returns the last written value.
3. During reset (`S_AXI_ARESETN = 0`), all registers are cleared to `0`, so all PWM outputs start inactive.
4. If a `DUTYx` value is greater than `PERIOD`, the internal counter never reaches that value before reset, so the corresponding output can remain high for almost the entire frame.
5. The timing examples in this document are based on the source definition `PERIOD = 2,000,000` and the source comment `20 ms @100 MHz`. If the actual system clock differs, the pulse width scales proportionally.
