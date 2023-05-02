#include "ControlSystem.hpp"

ControlSystem::ControlSystem(double dt)
    : myConstant(1.0), myGain(2.0),
      servoEncoder("quat1"),
      servoMotor("servo1"),
      timedomain("Main time domain", dt, true)
{
    // Name all blocks
    myConstant.setName("My constant");
    myGain.setName("My gain");
    servoEncoder.setName("Servo Encoder");
    servoMotor.setName("Servo Motor");
    controller.setName("Controller");

    // Name all signals
    myConstant.getOut().getSignal().setName("My constant value");
    myGain.getOut().getSignal().setName("My constant value multiplied with my gain");
    servoEncoder.getOut().getSignal().setName("phi [rad]");
    controller.getOut().getSignal().setName("U [V]");

    // Connect signals
    myGain.getIn().connect(myConstant.getOut());
    controller.getIn().connect(servoEncoder.getOut());
    servoMotor.getIn().connect(controller.getOut());

    // Add blocks to timedomain
    timedomain.addBlock(myConstant);
    timedomain.addBlock(myGain);
    timedomain.addBlock(servoEncoder);
    timedomain.addBlock(controller);
    timedomain.addBlock(servoMotor);

    // Add timedomain to executor
    eeros::Executor::instance().add(timedomain);
}