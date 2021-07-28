void tick()
{
  digitalWrite(LED, !digitalRead(LED));
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void turnMachineOn()
{
  digitalWrite(MAIN_RELAY, false);
}

void turnMachineOff()
{
  digitalWrite(MAIN_RELAY, true);
}