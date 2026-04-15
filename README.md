# Wombat-CC Arm Library

Arm is a C++ helper for controlling a shoulder/elbow/claw servo arm on KIPR Wombat.

It supports:

- 3-servo arms: shoulder + elbow + claw
- 2-servo arms: shoulder + claw
- Smoother servo movement using incremental steps
- Optional debug logging

## Add To A Project

Use this dependency key in your consumer `build.zig.zon`:

```zig
.dependencies = .{
    .wombat_cc_lib_arm = .{ .path = "../Wombat-CC-Arm/" },
};
```

Then include the header:

```cpp
#include <Arm.hpp>
```

## Quick Start

```cpp
#include <Arm.hpp>

int main()
{
    Arm arm(1, 2, 3); // shoulder, elbow, claw servo ports

    arm.SetDebugEnabled(true);

    arm.SetPosition(Arm::ArmPosition(
        1400, // shoulder target
        1200, // elbow target
        700   // claw target
    ));

    return 0;
}
```

2-servo example:

```cpp
Arm arm(1, 3); // shoulder, claw
arm.SetPosition(Arm::ArmPosition(1400, 700));
```

## API

### Construction

- `Arm(int shoulderServoPort, int elbowServoPort, int clawServoPort)`
- `Arm(int shoulderServoPort, int clawServoPort)`

### Position Type

- `Arm::ArmPosition(int shoulderPosition, int elbowPosition, int clawPosition)`
- `Arm::ArmPosition(int shoulderPosition, int clawPosition)`

### Motion

- `void SetPosition(const ArmPosition &position)`

### Debug

- `void SetDebugEnabled(bool enabled)`
- `bool IsDebugEnabled() const`

## Debug Logging

Two options are supported:

- Runtime toggle in code with `SetDebugEnabled(true)`
- Environment variable before program start: `WOMBAT_CC_DEBUG=1`

Truthy values currently recognized:

- `1`
- `true`
- `TRUE`
- `on`
- `ON`

## Behavior Notes

- `SetPosition` moves shoulder and claw in parallel.
- If elbow is configured and the provided position includes elbow, elbow also runs in parallel.
- Servo movement is incremental (step size `10`, delay `10 ms`) to reduce abrupt motion.
- If a servo is already at target, a warning is printed.

## Practical Tips

- Store tuned servo positions as named constants instead of raw literals.
- Tune shoulder/elbow/claw limits on hardware to avoid binding.
- Start with debug enabled while calibrating ports and positions.
