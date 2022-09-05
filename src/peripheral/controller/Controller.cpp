#include "Controller.h"

#include <Platform.h>

#include "../../level/Options.h"
#include "../../util/Math.h"

bool Controller::attack = 0;
int8_t Controller::angle = 0;

void Controller::init() {
    controller_init();
}

void Controller::callibrate() {
    controller_callibrate();
}

bool Controller::poll() {
    bool input   = false;
    int8_t angle = controller_poll_angle();
    bool attack  = controller_poll_attack();

    if (Options::controller_direction) {
        angle = 0 - angle;
    }
    if (angle != 0) {
        input = true;
        Controller::angle = angle * math::abs8(angle) / 100 + (angle > 0 ? 1 : -1);
    } else {
        Controller::angle = 0;
    }

    Controller::attack = attack;
    if (attack) {
        input = true;
    }

#if LOG_CONTROLLER
    LOG("Controller(a: ");
    LOG(getAngle());
    LOG(", w: ");
    LOG(""+isAttack());
    LOG_LN(")");
#endif

    return input;
}

int8_t Controller::getAngle() {
    return angle;
}

bool Controller::isAttack() {
    return attack;
}

bool Controller::pollOptions() {
    return controller_poll_options();
}
