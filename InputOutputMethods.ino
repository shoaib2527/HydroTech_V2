void tick()
{
  digitalWrite(LED, !digitalRead(LED));
}

void turnMachineOn()
{
  digitalWrite(MAIN_RELAY, false);
}

void turnMachineOff()
{
  digitalWrite(MAIN_RELAY, true);
}