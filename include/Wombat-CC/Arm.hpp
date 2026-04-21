//
// Created by Connor Denihan on 2/19/26.
//

/**
 * @file Arm.hpp
 * @brief Declares the Arm class and related types.
 */

#ifndef WOMBAT_CC_ARM_HPP
#define WOMBAT_CC_ARM_HPP

/**
 * @brief Controls a 3-servo arm (shoulder, elbow, claw).
 *
 * Stores the configured servo ports and provides a high-level API to set the
 * arm to a desired position.
 */
class Arm
{
private:
    /// Servo port for the shoulder joint.
    const int ShoulderServoPort;

    /// True if this arm is configured with an elbow servo.
    const bool HasElbow;

    /// Servo port for the elbow joint (only valid when HasElbow is true).
    const int ElbowServoPort;

    /// Servo port for the claw/gripper.
    const int ClawServoPort;

    /**
     * @brief Sets a servo to a specific position.
     * @param port Servo port to command.
     * @param position Target position value (units depend on servo
     * implementation). Intended for internal use.
     */
    void SetServoPosition(int port, int position);

public:
    /**
     * @brief Target arm configuration (shoulder, elbow, claw).
     */
    struct ArmPosition
    {
        /// Desired shoulder servo position.
        const int ShoulderPosition;

        /// Desired elbow servo position.
        const int ElbowPosition;

        /// Desired claw servo position.
        const int ClawPosition;

        /// True when this position includes an elbow target.
        const bool UsesElbow;

        /**
         * @brief Constructs a 3-joint arm position (shoulder, elbow, claw).
         */
        constexpr ArmPosition(int shoulderPosition, int elbowPosition,
                              int clawPosition)
            : ShoulderPosition(shoulderPosition), ElbowPosition(elbowPosition),
              ClawPosition(clawPosition), UsesElbow(true) {}

        /**
         * @brief Constructs a 2-joint arm position (shoulder, claw).
         */
        constexpr ArmPosition(int shoulderPosition, int clawPosition)
            : ShoulderPosition(shoulderPosition), ElbowPosition(0),
              ClawPosition(clawPosition), UsesElbow(false) {}
    };

    /**
     * @brief Moves the arm to the requested position.
     * @param position Target joint positions.
     * Multithreaded function that calls the SetServoPosition three times.
     */
    void SetPosition(ArmPosition position);

    /**
     * @brief Constructs an Arm using the provided servo ports.
     * @param shoulderServoPort Port for the shoulder servo.
     * @param elbowServoPort Port for the elbow servo.
     * @param clawServoPort Port for the claw servo.
     */
    Arm(int shoulderServoPort, int elbowServoPort, int clawServoPort);

    /**
     * @brief Constructs an Arm without an elbow servo.
     * @param shoulderServoPort Port for the shoulder servo.
     * @param clawServoPort Port for the claw servo.
     */
    Arm(int shoulderServoPort, int clawServoPort);

    // Destructor.
    ~Arm();
};

#endif // WOMBAT_CC_ARM_HPP
