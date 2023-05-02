#include "MyRobotSafetyProperties.hpp"

MyRobotSafetyProperties::MyRobotSafetyProperties(ControlSystem &cs, double dt)
    : cs(cs),
    
      slSystemOff("System is offline"),
      slShuttingDown("System is shutting down"),
      slSystemStartingUp("System is starting up"),
      slEmergency("System has detected an Emergency"),
      slSystemOn("System is online"),
      slMoving("System is Moving"),

      doSystemOff("Power off the system"),
      doShuttingDown("Shutting down the system"),
      doSystemStartingUp("Starting up the system"),
      doEmergency("Emerengy Detected"),
      doEmergencyResolved("Emerengy cleared"),
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
    slSystemOff.addEvent(doSystemStartingUp, slSystemStartingUp, kPublicEvent);
    slShuttingDown.addEvent(doSystemOff, slSystemOff, kPublicEvent);
    slSystemStartingUp.addEvent(doSystemOn, slSystemOn, kPublicEvent);
    slSystemStartingUp.addEvent(doEmergency, slEmergency, kPublicEvent);
    slSystemOn.addEvent(doStartMoving, slMoving, kPublicEvent);
    

    // Add events to multiple safety levels
    addEventToAllLevelsBetween(slSystemOn, slMoving, doEmergency, slEmergency, kPublicEvent);
    addEventToAllLevelsBetween(slEmergency, slMoving, doShuttingDown, slShuttingDown, kPublicEvent);

    // Define input actions for all levels
    slSystemOff.setInputActions({ignore(btnPause), ignore(btnMode) });
    slShuttingDown.setInputActions({ignore(btnPause), ignore(btnMode) });
    slSystemStartingUp.setInputActions({ignore(btnPause), check(btnMode, true, doEmergency) }); //goes to slEmergency
    slSystemOn.setInputActions({ignore(btnPause), check(btnMode, true, doEmergency) }); //goes to slEmergency
    slMoving.setInputActions({check(btnPause, true, doShuttingDown), check(btnMode, true, doEmergency) }); //goes to slEmergency
    slEmergency.setInputActions({check(btnPause, true, doEmergencyResolved), ignore(btnPause)});
    slSystemOn.setInputActions({check(btnPause, true, doStartMoving), ignore(btnPause)});

    // Define output actions for all levels
    slSystemOff.setOutputActions   (    { set(ledUser0, false), set(ledUser1, false), set(ledUser2, false), set(ledUser3, false)});
    slShuttingDown.setOutputActions(    { set(ledUser0, true),  set(ledUser1, false), set(ledUser2, false), set(ledUser3, false)});
    slSystemStartingUp.setOutputActions({ set(ledUser0, false), set(ledUser1, false), set(ledUser2, false), set(ledUser3, true) });
    slEmergency.setOutputActions(       { set(ledUser0, true),  set(ledUser1, true),  set(ledUser2, true),  set(ledUser3, true) });
    slSystemOn.setOutputActions(        { set(ledUser0, false), set(ledUser1, true),  set(ledUser2, true),  set(ledUser3, false)});
    slMoving.setOutputActions(          { set(ledUser0, true),  set(ledUser1, false), set(ledUser2, false), set(ledUser3, true) });

    // Define and add level actions
    slSystemOff.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.stop();
        eeros::Executor::stop();
    });

    slShuttingDown.setLevelAction([&](SafetyContext *privateContext){
        cs.timedomain.stop();
		privateContext->triggerEvent(doShuttingDown);
    });
    
    slSystemStartingUp.setLevelAction([&](SafetyContext *privateContext){
        privateContext->triggerEvent(doSystemStartingUp);
    });

    slEmergency.setLevelAction([&](SafetyContext *privateContext){
        //Nothing to write here, no event base action
    });

    slSystemOn.setLevelAction([&](SafetyContext *privateContext) {
        cs.timedomain.start();
        privateContext->triggerEvent(doStartMoving);
    });

    slMoving.setLevelAction([&](SafetyContext *privateContext){
        //TODO implement moving
    });

    // Define entry level
    setEntryLevel(slSystemOff);

    // Define exit function
    exitFunction = ([&](SafetyContext *privateContext) {
        privateContext->triggerEvent(doSystemOff);
    });
}
