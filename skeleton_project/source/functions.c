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

void performEmergencyStop(bool *orderList){
    elevio_motorDirection(DIRN_STOP);
    elevio_stopLamp(1);
    clearOrders(orderList);
}
 
void addOrder(int floor, bool *orderList){
    orderList[floor] = true;
}

void removeOrder(int floor, bool *orderList){
    orderList[floor] = false;
    turnOffAllFloorLamps(floor);
}

void clearOrders(bool *orderList){
    for (int i = 0; i < 4; i++){
        orderList[i] = false;
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
    elevio_doorOpenLamp(0);
    elevio_stopLamp(0);
    for(int floor = 0; floor < 3; floor++) {
        elevio_buttonLamp(floor, BUTTON_HALL_UP, 0);
        elevio_buttonLamp(floor+1, BUTTON_HALL_DOWN, 0);
        elevio_buttonLamp(floor, BUTTON_CAB, 0);
    }
    elevio_buttonLamp(3, BUTTON_CAB, 0);
    

    if (elevio_floorSensor()== -1){
        elevatorDown();
        while (elevio_floorSensor() == -1){
            //wait  
        }
        stop();   
    }
    if (elevio_floorSensor() != -1){
        elevio_floorIndicator(elevio_floorSensor());
    }
    return;
}
void  lookForOrders(bool *orderList){
    for (int floor = 0; floor < 4; floor++){
        if(elevio_callButton(floor, BUTTON_CAB)){
            elevio_buttonLamp(floor, BUTTON_CAB, 1);
            addOrder(floor, orderList);

        }
        if(floor < 3 && elevio_callButton(floor, BUTTON_HALL_UP)){
            elevio_buttonLamp(floor, BUTTON_HALL_UP, 1);
            addOrder(floor, orderList);
        }
        if(floor > 0 && elevio_callButton(floor, BUTTON_HALL_DOWN)){
            elevio_buttonLamp(floor, BUTTON_HALL_DOWN, 1);
            addOrder(floor, orderList);
        }
    }
}


void turnOffAllFloorLamps(int floor){
    switch (floor){
    case 0:
        elevio_buttonLamp(0, BUTTON_HALL_UP, 0);
        elevio_buttonLamp(0, BUTTON_CAB, 0);
        break;
    case 1:
        elevio_buttonLamp(1, BUTTON_HALL_DOWN, 0);
        elevio_buttonLamp(1, BUTTON_HALL_UP, 0);
        elevio_buttonLamp(1, BUTTON_CAB, 0);
        break;
    case 2:
        elevio_buttonLamp(2, BUTTON_HALL_DOWN, 0);
        elevio_buttonLamp(2, BUTTON_HALL_UP, 0);
        elevio_buttonLamp(2, BUTTON_CAB, 0);
        break;
    case 3:
        elevio_buttonLamp(3, BUTTON_HALL_DOWN, 0);
        elevio_buttonLamp(3, BUTTON_CAB, 0);
        break;
    }
}

bool isOrderListEmpty(bool *orderList){
    int count = 0;
    for(int i = 0; i < 4; i++){
        bool status = orderList[i];
        if(not(status)){
            count++;
        }
    }
    if(count == 4){
        return true;
    }
    return false;
}

bool isOrdersBelow(int currentFloor, bool *orderList){
    for (int floor = 0; floor < currentFloor; floor++){
        if(orderList[floor]){
            return true;
        }
    }
    return false;
}

bool isOrdersAbove(int currentFloor, bool *orderList){
    for (int floor = currentFloor; floor >= 0; floor--){
        if(orderList[floor]){
            return true;
        }
    }
    return false;
}