#pragma once

#include "elevio.h"
#include <stdbool.h>

void elevatorDown(void);
void elevatorUp(void);
void stop(void);
void performEmergencyStop(bool (*orderList)[4]); 
void addOrder(int floor, bool (*orderList)[4]);
void removeOrder(int floor, bool (*orderList)[4]);
void clearOrders(bool (*orderList)[4]);
void openDoor(void);
void closeDoor(void);
void initializeElevator(void);
void lookForOrders(bool (*orderList));