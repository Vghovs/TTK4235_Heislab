#include "functions.h"

void elevatorDown(void){
    elevio_motorDirection(DIRN_DOWN);
}

void elevatorUp(void){
    elevio_motorDirection(DIRN_UP);
}

void stop(void){
    elevio_motorDirection(DIRN_STOP);
}

void emergencyStop(void){
    elevio_motorDirection(DIRN_STOP);
    //floorQueueClear()
}

void addOrder(void);

void removeOrder(void);

void openDoor(void){
    elevio_doorOpenLamp(1);
    //1???????
}

void closeDoor(void){
    elevio_doorOpenLamp(0);
}

