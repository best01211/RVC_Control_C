#include <stdio.h>
#include <stdbool.h>

// Sensor input structure
typedef struct {
    bool front_sensor;
    bool left_sensor;
    bool right_sensor;
    bool dust_sensor;
    bool power_sensor;
} SensorInput;

// Motor commands
typedef enum {
    MOVE_FORWARD,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_AROUND,
    STOP
} MotorCommand;

// Cleaner control
typedef enum {
    CLEAN_ON,
    CLEAN_OFF
} CleanerControl;

// Obstacle & Dust Detection
void detect_obstacles_and_dust(SensorInput *input, bool *obstacle_detected, bool *dust_detected) {
    *obstacle_detected = input->front_sensor || input->left_sensor || input->right_sensor;
    *dust_detected = input->dust_sensor;
}

// Motor Control Logic
MotorCommand control_motor(bool obstacle_detected, bool dust_detected) {
    if (!obstacle_detected) return MOVE_FORWARD;
    if (dust_detected) return STOP; // Simulating cleaning action
    return TURN_AROUND; // Default turn-around action
}

// Cleaner Control Logic
CleanerControl control_cleaner(bool dust_detected) {
    return dust_detected ? CLEAN_ON : CLEAN_OFF;
}

int main() {
    // Simulated sensor inputs
    SensorInput sensor_input = {true, false, false, true, true};

    // Outputs
    bool obstacle_detected = false;
    bool dust_detected = false;
    MotorCommand motor_command;
    CleanerControl cleaner_control;

    // Detection
    detect_obstacles_and_dust(&sensor_input, &obstacle_detected, &dust_detected);

    // Control
    motor_command = control_motor(obstacle_detected, dust_detected);
    cleaner_control = control_cleaner(dust_detected);

    // Output results
    printf("Obstacle Detected: %s\\n", obstacle_detected ? "Yes" : "No");
    printf("Dust Detected: %s\\n", dust_detected ? "Yes" : "No");
    printf("Motor Command: %d\\n", motor_command);
    printf("Cleaner Control: %d\\n", cleaner_control);

    return 0;
}
