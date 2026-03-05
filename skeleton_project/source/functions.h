#pragma once

#include "driver/elevio.h"
#include <stdbool.h>

void elevatorDown(void);
void elevatorUp(void);
void stop(void);
void performEmergencyStop(bool *orderList1, bool *orderList2); 
void addOrder(int floor, bool *orderList);
void removeOrder(int floor, bool *orderListUp, bool *orderListDown);
void clearOrders(bool *orderList);
void openDoor(void);
void closeDoor(void);
void initializeElevator(void);
void lookForOrders(int currentFloor, bool *orderListUp, bool *orderListDown);
void turnOffAllFloorLamps(int floor);
bool isOrdersBelow(int currentFloor, bool *orderList);
bool isOrderListEmpty(bool *orderList);
bool isOrdersAbove(int currentFloor, bool *orderList);