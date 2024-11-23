#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Sensor input structure
typedef struct {
    bool front_sensor;  // Front sensor
    bool left_sensor;   // Left sensor
    bool right_sensor;  // Right sensor
    bool dust_sensor;   // Dust sensor
    bool power_sensor;  // Power sensor
    int time;           // Operating time (seconds)
} SensorInput;

// Motor commands
typedef enum {
    MOVE_FORWARD,  // Move forward
    TURN_LEFT,     // Turn left
    TURN_RIGHT,    // Turn right
    TURN_AROUND,   // Turn around (reverse)
    STOP           // Stop
} MotorCommand;

// Cleaner control
typedef enum {
    CLEANER_ON,    // Cleaner is ON
    CLEANER_OFF    // Cleaner is OFF
} CleanerControl;

// Cleaner power
typedef enum {
    CLEANER_POWER_UP,   // Turn cleaner power on
    CLEANER_POWER_DOWN  // Turn cleaner power off
} CleanerPower;

// Obstacle and dust detection
void detect_obstacles_and_dust(SensorInput *input, bool *obstacle_detected, bool *dust_detected) {
    *obstacle_detected = input->front_sensor || input->left_sensor || input->right_sensor;
    *dust_detected = input->dust_sensor;
}

// Motor control logic
MotorCommand control_motor(SensorInput *input) {
    if (!input->power_sensor || input->time > 300) {
        return STOP; // Stop if power is off or operating time exceeds 300 seconds
    }
    if (input->front_sensor && input->left_sensor && input->right_sensor) {
        return TURN_AROUND; // Turn around if all sensors detect obstacles
    }
    if (input->front_sensor && input->left_sensor) {
        return TURN_RIGHT; // Turn right if front and left sensors detect obstacles
    }
    if (input->front_sensor) {
        return TURN_LEFT; // Turn left if only the front sensor detects an obstacle
    }
    return MOVE_FORWARD; // Default action: move forward
}

// Cleaner control logic
CleanerControl control_cleaner(int time) {
    return time > 300 ? CLEANER_OFF : CLEANER_ON; // Cleaner off if time exceeds 300 seconds, otherwise ON
}

// Cleaner power logic
CleanerPower control_cleaner_power(bool dust_detected) {
    return dust_detected ? CLEANER_POWER_UP : CLEANER_POWER_DOWN; // Turn cleaner power on/off based on dust detection
}

// Convert motor command to text
const char* motor_command_to_text(MotorCommand command) {
    switch (command) {
        case MOVE_FORWARD: return "Move Forward";
        case TURN_LEFT: return "Turn Left";
        case TURN_RIGHT: return "Turn Right";
        case TURN_AROUND: return "Turn Around";
        case STOP: return "Stop";
        default: return "Unknown";
    }
}

// Convert cleaner control to text
const char* cleaner_control_to_text(CleanerControl control) {
    return control == CLEANER_ON ? "ON" : "OFF";
}

// Convert cleaner power to text
const char* cleaner_power_to_text(CleanerPower power) {
    return power == CLEANER_POWER_UP ? "UP" : "DOWN";
}

int main() {
    // Open test case file
    FILE *file = fopen("test_cases.txt", "r");
    if (!file) {
        printf("Error: Unable to open test_cases.txt\n");
        return 1;
    }

    char line[256];
    int case_num = 1;

    while (fgets(line, sizeof(line), file)) {
        SensorInput input;
        // Parse input data
        int parsed = sscanf(line, "Input: front_sensor = %d, left_sensor = %d, right_sensor = %d, dust_sensor = %d, power_sensor = %d, time = %d",
                            (int*)&input.front_sensor, (int*)&input.left_sensor, 
                            (int*)&input.right_sensor, (int*)&input.dust_sensor, 
                            (int*)&input.power_sensor, &input.time);

        if (parsed != 6) {
            printf("Error: Invalid input format in line: %s", line);
            continue;
        }

        // Initialize output variables
        bool obstacle_detected = false;
        bool dust_detected = false;
        MotorCommand motor_command;
        CleanerControl cleaner_control;
        CleanerPower cleaner_power;

        // Run detection logic
        detect_obstacles_and_dust(&input, &obstacle_detected, &dust_detected);

        // Run control logic
        motor_command = control_motor(&input);
        cleaner_control = control_cleaner(input.time);
        cleaner_power = control_cleaner_power(dust_detected);

        // Print results
        printf("====================================\n");
        printf("Test Case %d:\n", case_num++);
        printf("Input: front_sensor = %s, left_sensor = %s, right_sensor = %s, dust_sensor = %s, power_sensor = %s, time = %d seconds\n",
               input.front_sensor ? "ON" : "OFF",
               input.left_sensor ? "ON" : "OFF",
               input.right_sensor ? "ON" : "OFF",
               input.dust_sensor ? "ON" : "OFF",
               input.power_sensor ? "ON" : "OFF",
               input.time);
        printf("Obstacle Detected: %s\n", obstacle_detected ? "Yes" : "No");
        printf("Dust Detected: %s\n", dust_detected ? "Yes" : "No");
        printf("Motor Command: %s\n", motor_command_to_text(motor_command));
        printf("Cleaner Control: %s\n", cleaner_control_to_text(cleaner_control));
        printf("Cleaner Power: %s\n", cleaner_power_to_text(cleaner_power));
        printf("====================================\n\n");
    }

    fclose(file);
    return 0;
}
