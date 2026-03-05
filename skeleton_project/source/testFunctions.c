#include "functions.h"
#include "testFunctions.h"

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

bool testIsOrdersAbove(void){
    bool orders[4] = {false};
    bool *orderList = orders;

    for (int floor = 0; floor <=3; floor++) {
        if(isOrdersAbove(0, orderList)) {return false;}
    }

    orderList[2] = true;
    if(!isOrdersAbove(0, orderList)) {return false;}
    if(!isOrdersAbove(1, orderList)) {return false;}
    if(isOrdersAbove(2, orderList)) {return false;}
    if(isOrdersAbove(3, orderList)) {return false;}

    orderList[3] = true;
    for (int floor = 0; floor <=3; floor++) {
        if(!isOrdersAbove(0, orderList)) {return false;}
    }
    return true;
}

bool testIsOrdersBelow(void){
    bool orders[4] = {false};
    bool *orderList = orders;

    for (int floor = 0; floor <=3; floor++) {
        if(isOrdersBelow(0, orderList)) {return false;}
    }

    orderList[1] = true;
    if(isOrdersBelow(0, orderList)) {return false;}
    if(isOrdersBelow(1, orderList)) {return false;}
    if(!isOrdersBelow(2, orderList)) {return false;}
    if(!isOrdersBelow(3, orderList)) {return false;}

    orderList[0] = true;
    for (int floor = 0; floor <=3; floor++) {
        if(!isOrdersBelow(0, orderList)) {return false;}
    }
    return true;
}

bool testIsOrderlistEmpty(void) {
    bool orders[4] = {false};
    bool *orderList = orders;

    if(!isOrderListEmpty(orderList)) {return false;}

    orderList[0] = true;
    if(isOrderListEmpty(orderList)) {return false;}

    orderList[0] = false;
    orderList[3] = true;
    if(isOrderListEmpty(orderList)) {return false;}

    orderList[3] = false;
    orderList[1] = true;
    orderList[2] = true;
    if(isOrderListEmpty(orderList)) {return false;}

    return true;
}

bool testClearOrders(void) {
    bool orders[4] = {true};
    bool *orderList = orders;

    clearOrders(orderList);

    for (int floor = 0; floor <=3; floor++) {
        if(orderList[floor]) {return false;}
    }
    return true;
}

bool testRemoveOrders(void) {
    bool ordersUp[4] = {true};
    bool ordersDown[4] = {true};
    bool *orderListUp = ordersUp;
    bool *orderListDown = ordersDown;

    removeOrder(0, orderListUp, orderListDown);
    
    if(orderListDown[0] || orderListUp[0]) {return false;}
    for (int floor = 1; floor <=3; floor++) {
        if(!orderListDown[floor] || !orderListUp[floor]) {return false;}
    }

    removeOrder(3, orderListUp, orderListDown);

    if(orderListDown[0] || orderListUp[0]) {return false;}
    if(orderListDown[3] || orderListUp[3]) {return false;}
    for (int floor = 1; floor <=2; floor++) {
        if(!orderListDown[floor] || !orderListUp[floor]) {return false;}
    }

    removeOrder(1, orderListUp, orderListDown);
    removeOrder(2, orderListUp, orderListDown);

    for (int floor = 0; floor <= 3; floor++) {
        if(orderListDown[floor] || orderListUp[floor]) {return false;}
    }

    return true;
}