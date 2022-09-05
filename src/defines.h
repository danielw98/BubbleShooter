#ifndef _DEFINES_H_
#define _DEFINES_H_


#define WINDOW_WIDTH    1008
#define WINDOW_HEIGHT   753

#define SCORE_OFFSET_X  0
#define SCORE_OFFSET_Y  50
#define LEVEL_OFFSET_X  931
#define LEVEL_OFFSET_Y  631
#define BUBBLE_OFFSET_X 165
#define BUBBLE_OFFSET_Y -15
#define TIMER_OFFSET_X  900
#define TIMER_OFFSET_Y  135
#define BUBBLE_WIDTH    40
#define BUBBLE_HEIGHT   40
#define NUM_BUBBLES     5
#define NUM_LEVELS      50
#define BOARD_SIZE_X    18
#define BOARD_SIZE_Y    18
#define NUM_NODES       (BOARD_SIZE_X * BOARD_SIZE_Y)

#define CANNON_BUBBLE_OFFSET_X  480
#define CANNON_BUBBLE_OFFSET_Y  650
#define CANNON_BUBBLE_WIDTH     50
#define CANNON_BUBBLE_HEIGHT    50

#define SPARE_BUBBLE_OFFSET_X  30
#define SPARE_BUFFLE_OFFSET_Y 640
#define SPARE_BUBBLE_W         65
#define SPARE_BUBBLE_H         65

#define NUM_STRIKES     5
#define STRIKES_RECT_W  28
#define STRIKES_RECT_H  (125/NUM_STRIKES)
#define STRIKES_RECT_X  930
#define STRIKES_RECT_Y  405

#define WALL_LEFT_X   145
#define WALL_RIGHT_X  865
#define WALL_TOP_Y    0
#define WALL_BOTTOM_Y WINDOW_HEIGHT
// TODO: move these into structs when scaling to multiple levels
#define LVL1_START_X 4
#define LVL1_END_X   13
#define LVL1_START_Y 2
#define LVL1_END_Y   10

#define LVL1_CENTER_X 8
#define LVL1_CENTER_Y 8
#endif /* _DEFINES_H_ */