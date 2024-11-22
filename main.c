#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// 센서 입력 구조체
typedef struct {
    bool front_sensor;  // 전방 센서
    bool left_sensor;   // 좌측 센서
    bool right_sensor;  // 우측 센서
    bool dust_sensor;   // 먼지 센서
    bool power_sensor;  // 전원 센서
} SensorInput;

// 모터 명령
typedef enum {
    MOVE_FORWARD,  // 전진
    TURN_LEFT,     // 좌회전
    TURN_RIGHT,    // 우회전
    TURN_AROUND,   // 방향 전환 (후진)
    STOP           // 정지
} MotorCommand;

// 청소기 제어
typedef enum {
    CLEAN_ON,  // 청소기 켜기
    CLEAN_OFF  // 청소기 끄기
} CleanerControl;

// 장애물 및 먼지 감지
void detect_obstacles_and_dust(SensorInput *input, bool *obstacle_detected, bool *dust_detected) {
    *obstacle_detected = input->front_sensor || input->left_sensor || input->right_sensor;
    *dust_detected = input->dust_sensor;
}

// 모터 제어 로직
MotorCommand control_motor(SensorInput *input) {
    if (!input->power_sensor) {
        return STOP; // 전원이 꺼져 있으면 즉시 정지
    }
    if (input->front_sensor && input->left_sensor && input->right_sensor) {
        return TURN_AROUND; // 전방, 좌측, 우측 센서 모두 장애물 감지 시 방향 전환
    }
    if (input->front_sensor && input->left_sensor) {
        return TURN_RIGHT; // 전방, 좌측 센서 감지 시 우회전
    }
    if (input->front_sensor) {
        return TURN_LEFT; // 전방 센서만 감지 시 좌회전
    }
    return MOVE_FORWARD; // 기본 동작: 전진
}

// 청소기 제어 로직
CleanerControl control_cleaner(bool dust_detected) {
    return dust_detected ? CLEAN_ON : CLEAN_OFF; // 먼지 감지 시 청소기 켜기, 아니면 끄기
}

// 모터 명령을 텍스트로 변환
const char* motor_command_to_text(MotorCommand command) {
    switch (command) {
        case MOVE_FORWARD: return "MOVE_FORWARD";
        case TURN_LEFT: return "TURN_LEFT";
        case TURN_RIGHT: return "TURN_RIGHT";
        case TURN_AROUND: return "TURN_AROUND";
        case STOP: return "STOP";
        default: return "Unkown";
    }
}

// 청소기 제어를 텍스트로 변환
const char* cleaner_control_to_text(CleanerControl control) {
    return control == CLEAN_ON ? "Cleanner_ON" : "Cleanner_OFF";
}

int main() {
    // 테스트 케이스 파일 열기
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
        // "->" 이후의 출력 부분 제거
        char *output_part = strstr(line, "->");
        if (output_part) {
            *output_part = '\0'; // "->"부터 문자열을 자르기
        }

        SensorInput input;
        // 입력 데이터 파싱
        int parsed = sscanf(line, "Input: front_sensor = %d, left_sensor = %d, right_sensor = %d, dust_sensor = %d, power_sensor = %d",
                            (int*)&input.front_sensor, (int*)&input.left_sensor, 
                            (int*)&input.right_sensor, (int*)&input.dust_sensor, 
                            (int*)&input.power_sensor);

        if (parsed != 5) {
            printf("Error: Invalid input format in line: %s", line);
            continue; // 입력 형식이 잘못된 경우 해당 줄을 건너뜀
        }

        // 출력 변수 초기화
        bool obstacle_detected = false;
        bool dust_detected = false;
        MotorCommand motor_command;
        CleanerControl cleaner_control;

        // 감지 로직 실행
        detect_obstacles_and_dust(&input, &obstacle_detected, &dust_detected);

        // 제어 로직 실행
        motor_command = control_motor(&input);
        cleaner_control = control_cleaner(dust_detected);

        // 결과 출력
        printf("====================================\n");
        printf("Test Case %d:\n", case_num++);
        printf("Input: front_sensor = %s, left_sensor = %s, right_sensor = %s, dust_sensor = %s, power_sensor = %s\n",
               input.front_sensor ? "ON" : "OFF",
               input.left_sensor ? "ON" : "OFF",
               input.right_sensor ? "ON" : "OFF",
               input.dust_sensor ? "ON" : "OFF",
               input.power_sensor ? "ON" : "OFF");
        printf("Obstacle Detected: %s\n", obstacle_detected ? "Yes" : "No");
        printf("Dust Detected: %s\n", dust_detected ? "Yes" : "No");
        printf("Motor Command: %s\n", motor_command_to_text(motor_command));
        printf("Cleaner Control: %s\n", cleaner_control_to_text(cleaner_control));
        printf("====================================\n\n");
    }

    // 파일 닫기
    fclose(file);
    return 0;
}
