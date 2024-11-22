# RVC_Control_C
소프트웨어 공학 과제용 C  프로그래밍 개발

## 설명
1. 입력 데이터
- front_sensor, left_sensor, right_sensor, dust_sensor, power_sensor는 입력 값으로 사용됩니다.
- 각 센서의 값은 1(True) 또는 0(False)로 표현됩니다.
2. 출력 결과
- Obstacle Detected: 장애물이 탐지되었는지 여부를 판단합니다. front_sensor, left_sensor, right_sensor 중 하나라도 1이면 장애물이 있다고 판단합니다.
- Dust Detected: 먼지가 탐지되었는지 판단합니다. dust_sensor가 1이면 먼지가 있다고 판단합니다.
- Motor Command:
MOVE_FORWARD (0): 장애물이 없을 때 전진합니다.
TURN_AROUND (3): 장애물이 있고 먼지가 없을 때 방향을 바꿉니다.
STOP (4): 장애물이 있고 먼지가 있을 때 멈춥니다.
- Cleaner Control:
CLEAN_ON (1): 먼지가 있을 때 클리너 작동.
CLEAN_OFF (0): 먼지가 없을 때 클리너 비활성화.

## 패턴 및 특이점
장애물이 없고 먼지가 없는 경우:
항상 MOVE_FORWARD와 CLEAN_ON 명령을 수행합니다.
장애물이 있고 먼지가 있는 경우:
항상 STOP과 CLEAN_ON 명령을 수행합니다.
장애물이 있고 먼지가 없는 경우:
항상 TURN_AROUND 명령을 수행하며 CLEAN_OFF 상태를 유지합니다.