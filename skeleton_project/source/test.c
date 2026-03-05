#include "test.h"

bool testPreformEmergencyStop(void){
    bool arr1[4] = {true, true, false, true};
    bool arr2[4] = {false, true, true, false};
    bool *pointerArr1 = arr1;
    bool *pointerArr2 = arr2;
    performEmergencyStop(arr1, arr2);
    int listClearCounter = 0;
    for (int index = 0; index < 4; index++) {
        if (arr1[index] == false && arr2[index] == false) {
            ++listClearCounter;
        } 
    }
    if(listClearCounter == 4){
        return true;
    }
    return false;
    }


bool testAddOrder(void){
    bool arr1[4] = {0, 0, 0, 0};
    bool pointerArr1 = arr1;
    int counter = 0;
    for (int floor = 0; floor < 4; ++floor){
        addOrder(floor, arr1);
        if (arr1[floor] == true){
            counter++;
        }
        }
    if(counter == 4){
        return true;
    }
    return false;
}