void tick()
{
  digitalWrite(LED, !digitalRead(LED));
}


void turnMachineOn () {
  digitalWrite(MAIN_RELAY, true);
}


void turnMachineOff () {
  digitalWrite(MAIN_RELAY, false);
}
