#pragma once

#include "driver/elevio.h"
#include <stdbool.h>

void elevatorDown(void);
void elevatorUp(void);
void stop(void);
void performEmergencyStop(bool *orderList); 
void addOrder(int floor, bool *orderList);
void removeOrder(int floor, bool *orderList);
void clearOrders(bool *orderList);
void openDoor(void);
void closeDoor(void);
void initializeElevator(void);
void lookForOrders(bool *orderList);
void turnOffAllFloorLamps(int floor);
bool isOrdersBelow(int currentFloor, bool *orderList);
bool isOrderListEmpty(bool *orderList);
bool isOrdersAbove(int currentFloor, bool *orderList);