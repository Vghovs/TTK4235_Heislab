#pragma once
#include "functions.h"
#include "driver/elevio.h"
#include <stdbool.h>

bool testPreformEmergencyStop(void);
bool testAddOrder(void);
bool testIsOrdersAbove(void);
bool testIsOrdersBelow(void);
bool testIsOrderlistEmpty(void);
bool testClearOrders(void);
bool testRemoveOrders(void);
