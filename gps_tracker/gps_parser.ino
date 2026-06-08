void parseGPRMC(char line[])
{
  char copy[100];
  strcpy(copy, line);
  int fieldNum = 0;
  char *token = strtok(copy, ",");

  while (token != NULL)
  {
    if (fieldNum == 2)
      gStatus = token[0];
    if (fieldNum == 3)
      strcpy(gLatitude, token);
    if (fieldNum == 4)
      gNS = token[0];
    if (fieldNum == 5)
      strcpy(gLongitude, token);
    if (fieldNum == 6)
      gEW = token[0];
    fieldNum++;
    token = strtok(NULL, ",");
  }
}

void updateOLED()
{
  display.clearDisplay();
  if (gStatus == 'A')
  {
    display.setCursor(15,20);
    display.print("LAT: ");
    display.println(gLatitude);
    display.setCursor(15,35);
    display.print("LON: ");
    display.println(gLongitude);
    display.setCursor(15,50);
    display.println("FIX: YES");
  }
  else
  {
    display.setCursor(15, 20);
    display.println("FIX: NO");
  }
  display.display();
}