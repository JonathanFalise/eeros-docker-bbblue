#ifndef MyRobotSAFETYPROPERTIES_HPP_
#define MyRobotSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

class MyRobotSafetyProperties : public eeros::safety::SafetyProperties
{
public:
    MyRobotSafetyProperties(ControlSystem &cs, double dt);

    // Define all possible events
    eeros::safety::SafetyEvent doSystemOff;// bring robot to sl0
    eeros::safety::SafetyEvent doShuttingDown;// from any state to sl1 (slShuttingDown)
    eeros::safety::SafetyEvent doSystemStartingUp;// from sl0 boots to sl2
    eeros::safety::SafetyEvent doEmerency;// from any safety lvl puts robot to sl3
    eeros::safety::SafetyEvent doEmerencyResolved;// from sl3 puts robot to sl4 (slSystemOn)
    eeros::safety::SafetyEvent doSystemOn;// from sl2 to sl4 (slSystemOn)
    eeros::safety::SafetyEvent doStartMoving;// from sl4 puts robot to sl5 (slMoving)

    // Defina all possible levels
    eeros::safety::SafetyLevel slSystemOff;
    eeros::safety::SafetyLevel slShuttingDown;
    eeros::safety::SafetyLevel slSystemStartingUp;
    eeros::safety::SafetyLevel slEmergency;
    eeros::safety::SafetyLevel slSystemOn;
    eeros::safety::SafetyLevel slMoving;

private:
    // Define all critical outputs
    eeros::hal::Output<bool>* ledUser0;
    eeros::hal::Output<bool>* ledUser1;
    eeros::hal::Output<bool>* ledUser2;
    eeros::hal::Output<bool>* ledUser3;

    // Define all critical inputs
    eeros::hal::Input<bool>* btnPause;
    eeros::hal::Input<bool>* btnMode;

    ControlSystem &cs;
};

#endif // MyRobotSAFETYPROPERTIES_HPP_
