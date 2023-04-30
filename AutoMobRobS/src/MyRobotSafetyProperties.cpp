#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is offline"),
      slShuttingDown("System is shutting down"),
      slSystemStartingUp("System is starting up"),
      slEmergency("System has detected an Emergency"),
      slSystemOn("System is online"),
      slMoving("System is Moving"),

      doSystemOff("Shutdown the system"),
      doSystemStartingUp("Starting up the system"),
      doEmerency("Emerengy Detected"),
      doEmerencyResolved("Emerengy cleared"),
      doSystemOn("Startup the system"),
      doStartMoving("System is starting to move")
{
    eeros::hal::HAL &hal = eeros::hal::HAL::instance();

    // Declare and add critical outputs
    // ... = hal.getLogicOutput("...");

    // criticalOutputs = { ... };

    // Declare and add critical inputs
    // ... = eeros::hal::HAL::instance().getLogicInput("...", ...);

    // criticalInputs = { ... };

    // Add all safety levels to the safety system
    addLevel(slSystemOff);
    addLevel(slSystemOn);
    addLevel(slSystemStartingUp);
    addLevel(slEmergency);
    addLevel(slSystemOn);
    addLevel(slMoving);

    // Add events to individual safety levels
    slSystemOff.addEvent(doSystemOn, slSystemStartingUp, kPublicEvent);
    slSystemOn.addEvent(doSystemOff, slSystemOff, kPublicEvent);
    slSystemStartingUp.addEvent(doStartingUp, kPublicEvent);

    // Add events to multiple safety levels
    // addEventToAllLevelsBetween(lowerLevel, upperLevel, event, targetLevel, kPublicEvent/kPrivateEvent);

    // Define input actions for all levels
    // level.setInputActions({ ... });

    // Define output actions for all levels
    // level.setOutputActions({ ... });

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        eeros::Executor::stop();
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSystemOff);
    });
}
