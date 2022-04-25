// File Name: MidiIn.cpp
// Author: Akshat Madan
// Course: CS245
// Project: Assignment 05
// Date: 02/21/2022

#include "MidiIn.h"
#include <stdexcept>
#include <sstream>

std::string MidiIn::getDeviceInfo(void)
{
  std::string devinfo = "";
  int n = Pm_CountDevices();
  for (int i = 0; i < n; ++i)
  {
    const PmDeviceInfo *info = Pm_GetDeviceInfo(i); // Get device info for each device
    devinfo += i + '0';
    devinfo += ": ";
    devinfo += info->name;
    devinfo += "\n";
  }

  return devinfo; // Return all the device info
}

MidiIn::MidiIn(int devno)
{
  Pm_Initialize(); // Initialize PortMIDI
  PmError result = Pm_OpenInput(&input_stream, devno, 0, 64, 0, 0); // Open input device
  if (result != pmNoError)
    throw std::runtime_error("Error: Cannot open input device"); // Throw exception if something fails
  event_thread = new std::thread(eventLoop, this); // Start thread
  thread_running = true; // Start thread
  process_events = false; // Don't poll
}

void MidiIn::start()
{
  process_events = true; // Start polling
}

void MidiIn::stop()
{
  process_events = false; // Stop polling
}

MidiIn::~MidiIn(void)
{
  stop(); // Stop the polling
  thread_running = false;
  event_thread->join(); // Stop thread
  Pm_Close(input_stream); // Close PortMIDI Stream
  Pm_Terminate(); // Terminate PortMIDI
}

void MidiIn::eventLoop(MidiIn *midiin_ptr)
{
  while(midiin_ptr->thread_running)
  {
    if(midiin_ptr->process_events && Pm_Poll(midiin_ptr->input_stream))
    {
      // Fetch and process message
      union
      {
        long message;
        unsigned char byte[4];
      } msg;

      PmEvent event;
      Pm_Read(midiin_ptr->input_stream, &event, 1);
      msg.message = event.message;
      
      // Convert hex to string
      std::stringstream ss;
      ss << std::hex << (int)msg.byte[0];
      std::string temp = ss.str();

      int channel = (temp[1] >= 'a') ? (temp[1] - 'a' + 10) : (temp[1] - '0'); // Get channel number

      if(temp[0] == '9' && int(msg.byte[2]) != 0) // Note on
        midiin_ptr->onNoteOn(channel, int(msg.byte[1]), int(msg.byte[2]));

      if(temp[0] == '8' || (temp[0] == '9' && int(msg.byte[2]) == 0)) // Note off
        midiin_ptr->onNoteOff(channel, int(msg.byte[1]));

      if(temp[0] == 'e') // Pitch change
      {
        int val = (128 * int(msg.byte[2]))+ int(msg.byte[1]); // 2^7 * b2 + b1
        float pitch_change = ((float)val - 8192.0f) * (2.0f / 16383.0f); // Calculate floating point value
        midiin_ptr->onPitchWheelChange(channel, pitch_change);
      }

      if(temp[0] == 'b' && int(msg.byte[1]) == 7) // Volume change
        midiin_ptr->onVolumeChange(channel, int(msg.byte[2]));

      if(temp[0] == 'b' && int(msg.byte[1]) == 1) // Mod change
        midiin_ptr->onModulationWheelChange(channel, int(msg.byte[2]));

      if(temp[0] == 'b') // Control change
        midiin_ptr->onControlChange(channel, int(msg.byte[1]), int(msg.byte[2]));

      if(temp[0] == 'c') // Program (patch) change
        midiin_ptr->onPatchChange(channel, int(msg.byte[1]));
    }
  }
}
