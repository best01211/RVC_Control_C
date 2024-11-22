#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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
typedef enum { // ON == 1  OFF == 0
    CLEAN_OFF
    CLEAN_ON,
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
CleanerControl control_cleaner(bool dust_detected) { // return value?
    return dust_detected ? CLEAN_ON : CLEAN_OFF;
}

int main() {
    FILE *file = fopen("test_cases.txt", "r");
    if (!file) {
        printf("Error: Unable to open test_cases.txt\n");
        return 1;
    } else {
        printf("File opened successfully.\n");
    }

    char line[256];
    int case_num = 1;

    while (fgets(line, sizeof(line), file)) {
        // Remove output part if present
        char *output_part = strstr(line, "->");
        if (output_part) {
            *output_part = '\0'; // Truncate the string at the start of "->"
        }

        SensorInput input;
        int parsed = sscanf(line, "Input: front_sensor = %d, left_sensor = %d, right_sensor = %d, dust_sensor = %d, power_sensor = %d",
                            (int*)&input.front_sensor, (int*)&input.left_sensor, 
                            (int*)&input.right_sensor, (int*)&input.dust_sensor, 
                            (int*)&input.power_sensor);

        if (parsed != 5) {
            printf("Error: Invalid input format in line: %s", line);
            continue; // Skip this line if format is invalid
        }

        // Outputs
        bool obstacle_detected = false;
        bool dust_detected = false;
        MotorCommand motor_command;
        CleanerControl cleaner_control;

        // Detection
        detect_obstacles_and_dust(&input, &obstacle_detected, &dust_detected);

        // Control
        motor_command = control_motor(obstacle_detected, dust_detected);
        cleaner_control = control_cleaner(dust_detected);

        // Output results
        printf("Test Case %d:\n", case_num++);
        printf("Obstacle Detected: %s\n", obstacle_detected ? "Yes" : "No");
        printf("Dust Detected: %s\n", dust_detected ? "Yes" : "No");
        printf("Motor Command: %d\n", motor_command);
        printf("Cleaner Control: %d\n\n", cleaner_control);
    }

    fclose(file);
    return 0;
}
