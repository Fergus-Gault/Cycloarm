# Arduino + ROS 2 Robotic Arm — Full Development Plan

## Goal

Build a 6-DOF stepper-driven robotic arm using **two CNC control boards**, controlled from **ROS 2**, with support for:

- teleoperation (joint + Cartesian)
- motion recording and playback
- inverse kinematics (MoveIt 2)
- trajectory execution
- safety (limits, e-stop, watchdog)
- clean scaling and maintainability

Core principle:  
**ROS 2 is the brain. Arduinos are deterministic motion executors.**

---

## System Architecture (Target State)

### Hardware

- PC running Ubuntu + ROS 2 (Jazzy assumed)
- Arduino A + CNC Board A (3 joints)
- Arduino B + CNC Board B (3 joints)
- Stepper drivers (step/dir)
- Limit switches (at least shoulder + elbow)
- Shared motor enable line + physical e-stop

### Software Responsibilities

| Layer | Responsibility |
|-----|---------------|
| Arduino firmware | Step timing, homing, limits, trajectory execution |
| ROS 2 hardware interface | Converts ROS commands to board trajectories |
| ROS 2 control | Controllers, state publishing |
| MoveIt 2 | IK, planning, collision checking |
| Teleop + clips | Jogging, Cartesian control, record/playback |

---

## Phase 0 — Planning & Definitions (1–2 days)

### Decide and document

- Joint names and order (eg `joint_1` … `joint_6`)
- Board mapping (which joints on which Arduino)
- Steps-per-rad for each joint
- Gear ratios, microstepping
- Direction signs
- Soft limits for cable-wrap joints
- Max velocity and acceleration per joint
- Homing order (gravity-safe)

### Output

- `config/joints.yaml`
- `docs/architecture.md`

---

## Phase 1 — Single-Board Firmware Bring-Up (Board A) (3–7 days)

### Firmware features (minimum)

- Step/dir generation with acceleration limiting
- Internal step counters
- Homing with limit switches:
  - fast approach
  - back off
  - slow approach
- Soft limits
- Fault states
- Serial protocol:
  - `PING`
  - `GET_STATE`
  - `HOME(mask)`
  - `EXEC_TRAJ(points[])`
  - `STOP`
- Watchdog timeout

### Bench tests

- Repeatable homing
- Small moves and reversals
- Stall test → must fault, not drift silently

### Deliverable

Board A moves joints safely and repeatably.

---

## Phase 2 — Two-Board Synchronisation (3–5 days)

### Add to both firmwares

- Trajectory buffer
- `LOAD_TRAJ`
- `ARM(start_time)`
- `START`
- `BUSY` / `FAULT` flags

### Synchronisation strategy

- ROS uploads trajectories to both boards
- Boards wait for shared start time or simultaneous START
- Each board runs trajectory locally using its own timer

### Deliverable

Coordinated 6-joint motion with no visible desync.

---

## Phase 3 — ROS 2 Bring-Up (No ros2_control yet) (2–4 days)

### ROS nodes

- Serial interface node:
  - connects to both Arduinos
  - merges joint states
  - publishes `/joint_states`
- Services:
  - `/home_all`
  - `/stop`
  - `/jog_joint`

### Visualisation

- URDF / xacro
- `robot_state_publisher`
- RViz

### Deliverable

Arm moves in real life and RViz together.

---

## Phase 4 — ros2_control Integration (4–10 days)

### Implement

- Single `ros2_control` HardwareInterface:
  - owns 6 joints
  - internally talks to 2 serial ports
- `read()` → joint states from both boards
- `write()` → split trajectories and upload

### Controllers

- `joint_state_broadcaster`
- `joint_trajectory_controller`

### Deliverable

Standard ROS trajectory execution works.

---

## Phase 5 — MoveIt 2 (IK + Planning) (4–10 days)

### Tasks

- Generate MoveIt config package
- Define planning groups
- Configure joint limits
- Add collision geometry
- Test IK in RViz
- Plan + execute on hardware

### Deliverable

Pose-based planning works end-to-end.

---

## Phase 6 — Teleoperation (2–7 days)

### Step 1: Joint jog

- Gamepad or SpaceMouse
- Map axes → joint velocities
- Convert to short trajectories

### Step 2: Cartesian teleop

- MoveIt Servo
- Device → twist → IK → controller
- Tune smoothing and limits

### Deliverable

Smooth, safe teleop.

---

## Phase 7 — Motion Recording & Playback (2–7 days)

### Raw recording

- `rosbag2`:
  - `/joint_states`
  - teleop topics
  - trajectory commands

### Motion clips system

- Record joint states at fixed rate
- Save as `JointTrajectory` (YAML/JSON)
- Playback via `FollowJointTrajectory`
- Features:
  - retiming
  - speed scaling
  - go-to-start pose

### Deliverable

Repeatable, named motion clips.

---

## Phase 8 — Safety & Robustness (ongoing)

### Hardware safety

- Physical e-stop cuts motor enable
- Shared enable line if possible

### Software safety

- Watchdog on both boards
- Fault propagation: any fault stops both boards
- Re-home required after fault
- Soft limits enforced in firmware

### Optional upgrades

- Hall sensor index for continuous joints
- Encoders for missed-step detection

---

## Repository Structure

```bash
arm_stack/
├── docs/
├── config/
│ ├── joints.yaml
│ ├── controllers.yaml
│ └── teleop.yaml
├── firmware/
│ ├── common/
│ ├── board_a/
│ └── board_b/
├── ros2_ws/
│ └── src/
│ ├── arm_description/
│ ├── arm_bringup/
│ ├── arm_hw_interface/
│ ├── arm_teleop/
│ ├── arm_motion_clips/
│ └── arm_msgs/
├── tools/
│ ├── udev/
│ └── scripts/
└── README.md
```

---

## Milestones & Acceptance Tests

### M1 — Single board reliable

- 20 homing cycles
- No cumulative drift

### M2 — Two-board sync

- Coordinated 6-joint sweep
- No timing skew

### M3 — MoveIt execution

- 10 random reachable poses
- No stalls or limit hits

### M4 — Teleop + clips

- Record → playback matches motion

---

## Important Constraints (Reality Check)

- Stepper + step/dir ≠ torque control  
- “Inverse dynamics” = planning + limits + heuristics  
- True torque control requires different actuators

---

## Final Rule

**Do not add MoveIt or teleop until homing, limits, and e-stop are rock solid.**  
Everything else depends on that foundation.

