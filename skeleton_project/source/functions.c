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

void performEmergencyStop(bool (*orderList)[4]){
    elevio_motorDirection(DIRN_STOP);
    clearOrders(orderList);
}
 
void addOrder(int floor, bool (*orderList)[4]){
    switch(floor){
        case 1:
            (*orderList)[0] = true;
            break;
        case 2:
            (*orderList)[1] = true;
            break;
        case 3:
            (*orderList)[2] = true;
            break;
        case 4:
            (*orderList)[3] = true;
            break;
    } 
}

void removeOrder(int floor, bool (*orderList)[4]){
    switch(floor){
        case 1:
            (*orderList)[0] = false;
            break;
        case 2:
            (*orderList)[1] = false;
            break;
        case 3:
            (*orderList)[2] = false;
            break;
        case 4:
            (*orderList)[3] = false;
            break;
    } 
}

void clearOrders(bool (*orderList)[4]){
    for (int i = 0; i < 4; i++){
        (*orderList)[i] = false;
    }
}

void openDoor(void){
    elevio_doorOpenLamp(1);
}

void closeDoor(void){
    elevio_doorOpenLamp(0);
}

void initializeElevator(void){
    elevio_init();
    if (elevio_floorSensor()== -1){
        elevatorDown();
        while (elevio_floorSensor() == -1){
            //wait  
        }
        stop();   
    }
}


