void serial_read()
{
  //Serially read data from Onion Omega 2S
  if (Serial1.available())
  {
    vm_status = (Serial1.readString()).toInt();
    if(debug)
    {
      Serial.println("Serial Read: " + String(vm_status));
    }
  }
}

void serial_write(String buff)
{
  //Serially write data to Onion Omega 2S
  Serial1.write(buff.c_str());
  if(debug)
  {
    Serial.println("Serial Write: " + buff);
  }
}