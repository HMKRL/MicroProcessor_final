#include "point.h"

int POINT_getpoint(int line, int combo) {
    int point = 0;
    switch (line) {
        case 1:
            point = P_ONELINE * (100+combo*20)/100;
            break;
        case 2:
            point = P_TWOLINE * (100+combo*20)/100;
            break;
        case 3:
            point = P_THRLINE * (100+combo*20)/100;
            break;
        case 4:
            point = P_FORLINE * (100+combo*20)/100;
            break;
        default: break;
    }
    return point;
}
