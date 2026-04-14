//
// Created by Connor Denihan on 2/19/26.
//

#include <iostream>
#include <thread>
#include <kipr/wombat.h>
#include "../include/Arm.hpp"

// THREADED FUNCTION
void Arm::SetServoPosition(int port, int position)
{
    enable_servo(port);
    if (get_servo_position(port) < position)
    {
        for (int i = get_servo_position(port); i < position; i += 10)
        {
            set_servo_position(port, i);
            msleep(10);
        }
    }
    else if (get_servo_position(port) > position)
    {
        for (int i = get_servo_position(port); i > position; i -= 10)
        {
            set_servo_position(port, i);
            msleep(10);
        }
    }
    else
    {
        std::cout << "[WARNING]" << " " << "SERVO ALREADY AT POSTION" << std::endl;
    }
    disable_servo(port);
}

Arm::Arm(int shoulderServoPort, int elbowServoPort, int clawServoPort)
    : ShoulderServoPort(shoulderServoPort), HasElbow(true),
      ElbowServoPort(elbowServoPort), ClawServoPort(clawServoPort)
{
    // UpdateCurrentServoPositions();
}

Arm::Arm(int shoulderServoPort, int clawServoPort)
    : ShoulderServoPort(shoulderServoPort), HasElbow(false), ElbowServoPort(-1),
      ClawServoPort(clawServoPort)
{
    // UpdateCurrentServoPositions();
}

void Arm::SetPosition(ArmPosition position)
{
    // Init threads
    std::thread ShoulderThread(&Arm::SetServoPosition, this, ShoulderServoPort,
                               position.ShoulderPosition);
    std::thread ClawThread(&Arm::SetServoPosition, this, ClawServoPort,
                           position.ClawPosition);

    // Wait for threads to exit before returning
    if (HasElbow && position.UsesElbow)
    {
        std::thread ElbowThread(&Arm::SetServoPosition, this, ElbowServoPort,
                                position.ElbowPosition);
        ShoulderThread.join();
        ElbowThread.join();
        ClawThread.join();
        return;
    }

    ShoulderThread.join();
    ClawThread.join();
}

Arm::~Arm() = default;