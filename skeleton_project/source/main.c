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
                performEmergencyStop(orderList);
                break;
            }
            //look for userinput:
            lookForOrders(orderList);
            
            //determine where to go:
            for(int floor = 0; floor <=3; floor++){
                if(orderList[floor] && floor > currentFloor){
                    currentState = goingUp;
                    currentDirection = DIRN_UP;
                    printf("Change currentState to goingUp\n");
                    elevatorUp();
                    break;
                }
                else if (orderList[floor] && floor < currentFloor){
                    currentState = goingDown;
                    currentDirection = DIRN_DOWN;
                    printf("Change currentState to goingDown\n");
                    currentFloor--;
                    elevatorDown();
                    break;
                }
                else if (orderList[floor] && floor == currentFloor && elevio_floorSensor() == currentFloor){
                    currentState = doorOpen;
                    printf("Change currentState to doorOpen\n");
                }
                else if (orderList[floor] && floor == currentFloor && elevio_floorSensor() == -1){
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
                performEmergencyStop(orderList);
                break;
            }

            lookForOrders(orderList);

            tempFloor = elevio_floorSensor();
            if(tempFloor == -1){
                break;
            } else {
                currentFloor = tempFloor;
            }
            

            if(currentFloor >= 0 && currentFloor <= 3 && orderList[currentFloor]){
                stop();
                currentState = doorOpen;
                printf("Change currentState to doorOpen\n");
                removeOrder(currentFloor, orderList);
                openDoor();
                doorOpened = currentTime;
                break;
            }
            
            if (currentFloor==3 && (orderList[0]||orderList[1]||orderList[2])){ 
                currentState = goingDown;
                printf("Change currentState to goingDown");
                currentDirection = DIRN_DOWN;
                currentFloor--;
                elevatorDown();
                break;
            }
            else if(currentFloor==3){
                currentState = inactive;
                printf("Change currentState to inactive\n");
                currentDirection = DIRN_STOP;
                stop();
                break;
            }

            break;


        case goingDown:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                printf("Change currentState to emergencyStop\n");
                performEmergencyStop(orderList);
                break;
            }

            lookForOrders(orderList);

            tempFloor = elevio_floorSensor();
            if(tempFloor == -1){
                break;
            } else {
                currentFloor = tempFloor;
            }
            

            if(currentFloor >= 0 && currentFloor <= 3 && orderList[currentFloor]){
                stop();
                currentState = doorOpen;
                removeOrder(currentFloor, orderList);
                openDoor();
                doorOpened = currentTime;
                break;
            }
            else if(currentFloor > 0){
                currentFloor--;
                break;
            }
            
            if (currentFloor==0 && (orderList[1]||orderList[2]||orderList[3])){ 
                currentState = goingUp;
                printf("Change currentState to goingUp\n");
                currentDirection = DIRN_UP;
                elevatorUp();
                break;
            }
            else if(currentFloor==0){
                currentState = inactive;
                currentDirection = DIRN_STOP;
                stop();
                break;
            }

            break;

        case doorOpen:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                performEmergencyStop(orderList);
                break;
            }

            if(elevio_obstruction()){
                currentState = obstruction;
                doorOpened = 0;
                break;
            }

            lookForOrders(orderList);
            
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
                removeOrder(currentFloor, orderList);
            }
            if (isOrderListEmpty(orderList)) {
                currentState = inactive;
                currentDirection = DIRN_STOP;
            }
            else if (currentDirection == DIRN_DOWN && isOrdersBelow(currentFloor, orderList)) {
                currentState = goingDown;
                elevatorDown();
            }
            else if (currentDirection == DIRN_UP && isOrdersAbove(currentFloor, orderList)) {
                currentState = goingUp;
                elevatorUp();
            }
            else if (isOrdersBelow(currentFloor, orderList)) {
                currentState = goingDown;
                currentDirection = DIRN_DOWN;
                elevatorDown();
            }
            else {
                currentState = goingUp;
                currentDirection = DIRN_UP;
                elevatorUp();
            }
            
            

            break;

        case obstruction:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                currentDirection = DIRN_STOP;
                performEmergencyStop(orderList);
                break;
            }
            lookForOrders(orderList);

            if(elevio_obstruction() != 1){
                currentState = doorOpen;
                doorOpened = currentTime;
                break;
            }
        }
    }
}
