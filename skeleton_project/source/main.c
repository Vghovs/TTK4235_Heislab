#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include "driver/elevio.h"
#include "functions.h"

//Fininte state machine states:
#define initiate 0
#define inactive 1
#define emergencyStop 2
#define goingUp 3
#define goingDown 4
#define doorOpen 5
#define obstruction 6



int main(){
    bool (*orderList)[4] = {0};
    int currentState = initiate;

    initializeElevator();
    int currentFloor = elevio_floorSensor();

    currentState = inactive;

    while(1){
        switch (currentState){
        case inactive:
            //checks for ememgency stop:
            if(elevio_stopButton){
                currentState = emergencyStop;
                performEmergencyStop(orderList);
                break;
            }
            //look for userinput:
            lookForOrders(orderList);
            
            //determine where to go:
            for(int floor = 0; floor <=3; floor++){
                if((*orderList)[floor] && floor > currentFloor){
                    currentState = goingUp;
                }
                else if ((*orderList)[floor] && floor < currentFloor){
                    currentState = goingDown;
                    currentFloor--;
                }
                else if ((*orderList)[floor] && floor == currentFloor && elevio_floorSensor() == currentFloor){
                    currentState = doorOpen;
                }
                else if ((*orderList)[floor] && floor == currentFloor && elevio_floorSensor() == -1){
                    currentState = goingDown;
                }
            }
            break;

        case emergencyStop:
            if(!elevio_stopButton()){
                currentState = inactive;
            }
            break;

        case goingUp:
            //checks for ememgency stop:
            if(elevio_stopButton){
                currentState = emergencyStop;
                performEmergencyStop(orderList);
                break;
            }

            lookForOrders(orderList);

            if(elevio_floorSensor() == -1){
                break;
            }
            
            currentFloor = elevio_floorSensor();

            if((*orderList)[currentFloor]){
                
            }


        case goingDown:

            break;

        case doorOpen:

            break;

        case obstruction:

            break;

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
