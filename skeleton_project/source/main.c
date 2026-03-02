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

bool orders[4] = {false};
bool *orderList = orders;
int currentState = initiate;
int currentDirection = DIRN_STOP;
int currentFloor = 0;
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
        if (elevio_floorSensor() >= 0 && elevio_floorSensor() >= 0) {elevio_floorIndicator(elevio_floorSensor());};

        switch (currentState){
        case inactive:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                performEmergencyStop(orderList);
                break;
            }
            //look for userinput:
            lookForOrders(orderList);
            
            //determine where to go:
            for(int floor = 0; floor <=3; floor++){
                if(orderList[floor] && floor > currentFloor){
                    currentState = goingUp;
                    printf("Change currentState to goingUp");
                    elevatorUp();
                    break;
                }
                else if (orderList[floor] && floor < currentFloor){
                    currentState = goingDown;
                    printf("Change currentState to goingDown");
                    currentFloor--;
                    elevatorDown();
                    break;
                }
                else if (orderList[floor] && floor == currentFloor && elevio_floorSensor() == currentFloor){
                    currentState = doorOpen;
                    printf("Change currentState to doorOpen");
                }
                else if (orderList[floor] && floor == currentFloor && elevio_floorSensor() == -1){
                    currentState = goingDown;
                    printf("Change currentState to goingDown");
                }
            }
            break;

        case emergencyStop:
            if(!elevio_stopButton()){
                elevio_stopLamp(0);
                currentState = inactive;
                printf("Change currentState to inactive");
            }
            break;

        case goingUp:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
                printf("Change currentState to emergencyStop");
                performEmergencyStop(orderList);
                break;
            }

            lookForOrders(orderList);

            if(elevio_floorSensor() == -1){
                break;
            }
            
            currentFloor = elevio_floorSensor();

            if(currentFloor >= 0 && currentFloor <= 3 && orderList[currentFloor]){
                stop();
                currentState = doorOpen;
                printf("Change currentState to doorOpen");
                orderList[currentFloor] = 0;
                openDoor();
                doorOpened = currentTime;
                break;
            }
            
            if (currentFloor==3 && (orderList[0]||orderList[1]||orderList[2])){ //if we are at the top and there are orders below
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
                printf("Change currentState to emergencyStop\n");
                performEmergencyStop(orderList);
                break;
            }

            lookForOrders(orderList);

            int tempFloor = elevio_floorSensor();
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
            removeOrder(elevio_floorSensor(), orderList);
            if (isOrderListEmpty(orderList)) {
                currentState = inactive;
                currentDirection = DIRN_STOP;
            }
            else if (currentDirection == DIRN_DOWN && isOrdersBelow(currentFloor, orderList)) {
                currentState = goingDown;
            }
            else if (currentDirection == DIRN_UP && isOrdersAbove(currentFloor, orderList)) {
                currentState = goingUp;
            }
            else if (isOrdersBelow(currentFloor, orderList)) {
                currentState = goingDown;
            }
            else {
                currentState = goingUp;
            }
            
            

            break;

        case obstruction:
            //checks for ememgency stop:
            if(elevio_stopButton()){
                currentState = emergencyStop;
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








// int main(){
//     elevio_init();
    
//     printf("=== Example Program ===\n");
//     printf("Press the stop button on the elevator panel to exit\n");

//     elevio_motorDirection(DIRN_UP);

//     while(1){
//         int floor = elevio_floorSensor();

//         if(floor == 0){
//             elevio_motorDirection(DIRN_UP);
//         }

//         if(floor == N_FLOORS-1){
//             elevio_motorDirection(DIRN_DOWN);
//         }


//         for(int f = 0; f < N_FLOORS; f++){
//             for(int b = 0; b < N_BUTTONS; b++){
//                 int btnPressed = elevio_callButton(f, b);
//                 elevio_buttonLamp(f, b, btnPressed);
//             }
//         }

//         if(elevio_obstruction()){
//             elevio_stopLamp(1);
//         } else {
//             elevio_stopLamp(0);
//         }
        
//         if(elevio_stopButton()){
//             elevio_motorDirection(DIRN_STOP);
//             break;
//         }
        
//         nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
//     }

//     return 0;
// }
