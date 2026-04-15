//
// Created by Connor Denihan on 2/19/26.
//

#include <iostream>
#include <thread>
#include <cstdlib>
#include <string_view>
#include <kipr/wombat.h>
#include "../include/Arm.hpp"

namespace
{
    constexpr int kServoStep = 10;
    constexpr int kServoDelayMs = 10;

    bool IsTruthyDebugValue(const char *value)
    {
        if (value == nullptr)
        {
            return false;
        }

        const std::string_view raw_value(value);
        return raw_value == "1" || raw_value == "true" || raw_value == "TRUE" ||
               raw_value == "on" || raw_value == "ON";
    }

    bool IsDebugEnabledFromEnvironment()
    {
        return IsTruthyDebugValue(std::getenv("WOMBAT_CC_DEBUG"));
    }
} // namespace

void Arm::DebugLog(const char *message) const
{
    if (!DebugEnabled)
    {
        return;
    }

    std::cout << "[ARM][DEBUG] " << message << std::endl;
}

void Arm::DebugLogServoMove(int port, int from_position, int to_position) const
{
    if (!DebugEnabled)
    {
        return;
    }

    std::cout << "[ARM][DEBUG] Servo port=" << port
              << " from=" << from_position << " to=" << to_position
              << std::endl;
}

// THREADED FUNCTION
void Arm::MoveServoToPosition(int port, int position)
{
    enable_servo(port);
    const int current_position = get_servo_position(port);

    DebugLogServoMove(port, current_position, position);

    if (current_position < position)
    {
        for (int i = current_position; i < position; i += kServoStep)
        {
            set_servo_position(port, i);
            msleep(kServoDelayMs);
        }
    }
    else if (current_position > position)
    {
        for (int i = current_position; i > position; i -= kServoStep)
        {
            set_servo_position(port, i);
            msleep(kServoDelayMs);
        }
    }
    else
    {
        std::cout << "[WARNING] SERVO ALREADY AT POSITION" << std::endl;
    }
    disable_servo(port);
}

Arm::Arm(int shoulderServoPort, int elbowServoPort, int clawServoPort)
    : ShoulderServoPort(shoulderServoPort), HasElbow(true),
      ElbowServoPort(elbowServoPort), ClawServoPort(clawServoPort),
      DebugEnabled(IsDebugEnabledFromEnvironment())
{
    if (DebugEnabled)
    {
        DebugLog("Debug mode enabled via WOMBAT_CC_DEBUG.");
    }
}

Arm::Arm(int shoulderServoPort, int clawServoPort)
    : ShoulderServoPort(shoulderServoPort), HasElbow(false), ElbowServoPort(-1),
      ClawServoPort(clawServoPort), DebugEnabled(IsDebugEnabledFromEnvironment())
{
    if (DebugEnabled)
    {
        DebugLog("Debug mode enabled via WOMBAT_CC_DEBUG.");
    }
}

void Arm::SetDebugEnabled(bool enabled)
{
    DebugEnabled = enabled;
}

bool Arm::IsDebugEnabled() const
{
    return DebugEnabled;
}

void Arm::SetPosition(const ArmPosition &position)
{
    DebugLog("SetPosition called.");

    // Start shoulder and claw movement in parallel.
    std::thread shoulder_thread(&Arm::MoveServoToPosition, this, ShoulderServoPort,
                                position.ShoulderPosition);
    std::thread claw_thread(&Arm::MoveServoToPosition, this, ClawServoPort,
                            position.ClawPosition);

    const bool should_move_elbow = HasElbow && position.UsesElbow;
    std::thread elbow_thread;
    if (should_move_elbow)
    {
        elbow_thread = std::thread(&Arm::MoveServoToPosition, this, ElbowServoPort,
                                   position.ElbowPosition);
    }

    shoulder_thread.join();
    if (should_move_elbow)
    {
        elbow_thread.join();
    }
    claw_thread.join();
}

Arm::~Arm() = default;