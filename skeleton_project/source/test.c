#include <assert.h>
#include "testFunctions.h"

void main() {
    assert(testPreformEmergencyStop());
    assert(testAddOrder());
    assert(testIsOrdersAbove());
    assert(testIsOrdersBelow());
    assert(testIsOrderlistEmpty());
    assert(testClearOrders());
    assert(testRemoveOrders());
    printf("All tests passed!");
}