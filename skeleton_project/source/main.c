#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include "driver/elevio.h"
#include "functions.h"

//Fininte state machine states:
typedef enum {
    initiate = 0,
    inactive = 1,
    emergencyStop = 2,
    goingUp = 3,
    goingDown = 4,
    doorOpen  = 5,
    obstruction  = 6,
}FSMStates;

bool ordersUp[4] = {false};
bool ordersDown[4] = {false};
bool *orderListUp = ordersUp;
bool *orderListDown = ordersDown;
int currentState = initiate;
int currentDirection = DIRN_STOP;
int currentFloor = 0;
int tempFloor = 0;
time_t currentTime = 0;
time_t doorOpened = 0;



int main(){
    printf("Initializing");
    initializeElevator();
    currentFloor = elevio_floorSensor();

    currentState = inactive;
    currentDirection = DIRN_DOWN;
    

    while(true){
        currentTime = time(NULL);

        tempFloor = elevio_floorSensor();
            if(tempFloor != -1){
                elevio_floorIndicator(tempFloor);
            }
            
        switch (currentState){
        case inactive:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                performEmergencyStop(orderListUp, orderListDown);
                break;
            }
            //look for userinput:
            lookForOrders(currentFloor, orderListUp, orderListDown);
            
            //determine where to go:
            for(int floor = 0; floor <=3; floor++){
                if((orderListUp[floor] || orderListDown[floor]) && floor > currentFloor){
                    currentState = goingUp;
                    currentDirection = DIRN_UP;
                    printf("Change currentState to goingUp\n");
                    elevatorUp();
                    break;
                }
                else if ((orderListUp[floor] || orderListDown[floor]) && floor < currentFloor){
                    currentState = goingDown;
                    currentDirection = DIRN_DOWN;
                    printf("Change currentState to goingDown\n");
                    currentFloor--;
                    elevatorDown();
                    break;
                }
                else if ((orderListUp[floor] || orderListDown[floor]) && floor == currentFloor && elevio_floorSensor() == currentFloor){
                    currentState = doorOpen;
                    openDoor();
                    printf("Change currentState to doorOpen\n");
                }
                else if ((orderListUp[floor] || orderListDown[floor]) && floor == currentFloor && elevio_floorSensor() == -1){
                    currentState = goingDown;
                    currentDirection = DIRN_DOWN;
                    elevatorDown();
                    printf("Change currentState to goingDown\n");
                }
            }
            break;

        case emergencyStop:
            if (elevio_floorSensor()!=-1){
                openDoor();
            }
            if(!elevio_stopButton()){
                elevio_stopLamp(0);
                if(elevio_floorSensor()!=-1) {
                    currentState = doorOpen;
                    printf("Change currentState to doorOpen\n");
                } else {
                    currentState = inactive;
                    printf("Change currentState to inactive\n");
                }
            }
            break;

        case goingUp:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                printf("Change currentState to emergencyStop\n");
                performEmergencyStop(orderListUp, orderListDown);
                break;
            }

            lookForOrders(currentFloor, orderListUp, orderListDown);

            tempFloor = elevio_floorSensor();
            if(tempFloor >= 0 && tempFloor <= 3 ){
                currentFloor = tempFloor;
            } else {
                break;
            }
            
            if(orderListUp[currentFloor]){
                stop();
                currentState = doorOpen;
                printf("Change currentState to doorOpen\n");
                removeOrder(currentFloor, orderListUp, orderListDown);
                openDoor();
                doorOpened = currentTime;
                break;
            }
            else if (!(isOrdersAbove(currentFloor, orderListUp) || isOrdersAbove(currentFloor, orderListDown))) {
                stop();
                currentState = doorOpen;
                openDoor();
                doorOpened = currentTime;
                break;
            }
            
            break;

        case goingDown:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                printf("Change currentState to emergencyStop\n");
                performEmergencyStop(orderListUp, orderListDown);
                break;
            }

            lookForOrders(currentFloor, orderListUp, orderListDown);

            tempFloor = elevio_floorSensor();
            if(tempFloor >= 0 && tempFloor <= 3 ){
                currentFloor = tempFloor;
            } else {
                break;
            }
            

            if(orderListDown[currentFloor]){
                stop();
                currentState = doorOpen;
                removeOrder(currentFloor, orderListUp, orderListDown);
                openDoor();
                doorOpened = currentTime;
                break;
            }
            else if(!(isOrdersBelow(currentFloor, orderListUp) || isOrdersBelow(currentFloor, orderListDown))){
                stop();
                currentState = doorOpen;
                printf("Change currentState to doorOpen\n");
                openDoor();
                doorOpened = currentTime;
                break;
            }
            else if(currentFloor > 0){
                currentFloor--;
                break;
            }
            
            break;

        case doorOpen:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                performEmergencyStop(orderListUp, orderListDown);
                break;
            }

            if(elevio_obstruction()){
                currentState = obstruction;
                doorOpened = 0;
                break;
            }

            lookForOrders(currentFloor, orderListUp, orderListDown);
            
            if(doorOpened == 0){
                doorOpened = currentTime;
                break;
            }
            else if (currentTime-doorOpened <= 3){
                break;
            }

            closeDoor();
            doorOpened = 0;
            if(currentFloor >= 0 && currentFloor <= 3) {
                removeOrder(currentFloor, orderListUp, orderListDown);
            }
            if (isOrderListEmpty(orderListUp) && isOrderListEmpty(orderListDown)) {
                currentState = inactive;
                currentDirection = DIRN_STOP;
                break;
            }
            else if (currentDirection == DIRN_STOP) {
                currentDirection = DIRN_DOWN;
            }
            
            if (currentDirection == DIRN_DOWN && isOrdersBelow(currentFloor, orderListDown)) {
                currentState = goingDown;
                elevatorDown();
            }
            else if (currentDirection == DIRN_UP && isOrdersAbove(currentFloor, orderListUp)) {
                currentState = goingUp;
                elevatorUp();
            }
            else if (currentDirection == DIRN_DOWN && isOrdersBelow(currentFloor, orderListUp)) {
                currentState = goingDown;
                elevatorDown();
            }
            else if (currentDirection == DIRN_UP && isOrdersAbove(currentFloor, orderListDown)) {
                currentState = goingUp;
                elevatorUp();
            }
            else if (currentDirection == DIRN_DOWN && isOrdersAbove(currentFloor, orderListUp)) {
                currentState = goingUp;
                currentDirection = DIRN_UP;
                elevatorUp();
            }
            else if (currentDirection == DIRN_UP && isOrdersBelow(currentFloor, orderListDown)) {
                currentState = goingDown;
                currentDirection = DIRN_DOWN;
                elevatorDown();
            }
            else if (currentDirection == DIRN_DOWN && isOrdersAbove(currentFloor, orderListDown)) {
                currentState = goingUp;
                currentDirection = DIRN_UP;
                elevatorUp();
            }
            else if (currentDirection == DIRN_UP && isOrdersBelow(currentFloor, orderListUp)) {
                currentState = goingDown;
                currentDirection = DIRN_DOWN;
                elevatorDown();
            }
            
            break;

        case obstruction:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                performEmergencyStop(orderListUp, orderListDown);
                break;
            }
            lookForOrders(currentFloor, orderListUp, orderListDown);

            if(elevio_obstruction() != 1){
                currentState = doorOpen;
                doorOpened = currentTime;
                break;
            }
        }
    }
}
