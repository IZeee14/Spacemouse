#include <Arduino.h>
#include "config.h"
#include "hidInterface.h"
// Include inbuilt Arduino HID library by NicoHood: https://github.com/NicoHood/HID
#include "HID.h"

// Send a HID report every 8 ms (found on an original SpaceNavigator)
#define HIDUPDATERATE_MS 8

// little state machine to track, which report to send next
enum States
{
  ST_START,     // start to check if something is to be sent
  ST_SENDTRANS, // send translations
  ST_SENDROT,   // send rotations
  ST_SENDKEYS   // send keys
};

States nextState = ST_START; // init state machine with start state

#if (NUMKEYS > 0)
// Array with the bitnumbers, which should assign keys to buttons
uint8_t bitNumber[NUMHIDKEYS] = BUTTONLIST;

void prepareKeyBytes(uint8_t *keys, uint8_t *keyData, int debug);
#endif

uint8_t countTransZeros = 0; // count how many times, the zero data has been sent
uint8_t countRotZeros = 0;

// Function to send translation and rotation data to the 3DConnexion software using the HID protocol
// Three sets of data are sent: translation, rotation and key events
// For each, a 16bit integer is split into two using bit shifting. The first is mangitude and the second is direction.
// With regard to the _hidReportDescriptor: the values should be within +/- 350.
// returns true, if new data was sent
// A HID report may be sent every HIDUPDATERATE_MS, which should be 8 ms
bool send_command(int16_t rx, int16_t ry, int16_t rz, int16_t x, int16_t y, int16_t z, uint8_t *keys, int debug)
{
  static unsigned long now = 0; // time from millis()
  now = millis();

  static unsigned long lastHIDsentRep = now; // time from millis(), when the last HID report was sent

  bool hasSentNewData = false; // this value will be returned

#if (NUMKEYS > 0)
  static uint8_t keyData[HIDMAXBUTTONS / 8];     // key data to be sent via HID
  static uint8_t prevKeyData[HIDMAXBUTTONS / 8]; // previous key data
  prepareKeyBytes(keys, keyData, debug);         // sort the bytes from keys into the bits in keyData
#endif

  switch (nextState) // state machine
  {
  case ST_START:
    // Evaluate everytime, without waiting for 8ms
    if (countTransZeros < 3 || countRotZeros < 3 || (x != 0 || y != 0 || z != 0 || rx != 0 || ry != 0 || rz != 0))
    {
      // if one of the values is not zero,
      // or not all zero data packages are sent (sent 3 of them)
      // start sending data
      nextState = ST_SENDTRANS;
    }
    else
    {
// if nothing is to be sent, check for keys. If no keys, don't change state
#if (NUMKEYS > 0)
      if (memcmp(keyData, prevKeyData, HIDMAXBUTTONS / 8) != 0)
      // compare key data to previous key data
      {
        nextState = ST_SENDKEYS;
      }
#endif
      if (nextState == ST_START && (now - lastHIDsentRep >= HIDUPDATERATE_MS))
      {
        // if we are not leaving the start state and
        // we are waiting here for more than the update rate,
        // keep the timestamp for the last sent package nearby
        lastHIDsentRep = now - HIDUPDATERATE_MS;
      }
    }
    break;
  case ST_SENDTRANS:
    // send translation data, if the 8 ms from the last hid report have past
    if (now - lastHIDsentRep >= HIDUPDATERATE_MS)
    {
      uint8_t trans[6] = {(byte)(x & 0xFF), (byte)(x >> 8), (byte)(y & 0xFF), (byte)(y >> 8), (byte)(z & 0xFF), (byte)(z >> 8)};
      HID().SendReport(1, trans, 6); // send new translational values
      lastHIDsentRep += HIDUPDATERATE_MS;
      hasSentNewData = true; // return value

      // if only zeros where send, increment zero counter, otherwise reset it
      if (x == 0 && y == 0 && rz == 0)
      {
        countTransZeros++;
      }
      else
      {
        countTransZeros = 0;
      }
      nextState = ST_SENDROT;
    }
    break;
  case ST_SENDROT:
    // send rotational data, if the 8 ms from the last hid report have past
    if (now - lastHIDsentRep >= HIDUPDATERATE_MS)
    {
      uint8_t rot[6] = {(byte)(rx & 0xFF), (byte)(rx >> 8), (byte)(ry & 0xFF), (byte)(ry >> 8), (byte)(rz & 0xFF), (byte)(rz >> 8)};
      HID().SendReport(2, rot, 6);
      lastHIDsentRep += HIDUPDATERATE_MS;
      hasSentNewData = true; // return value
      // if only zeros where send, increment zero counter, otherwise reset it
      if (rx == 0 && ry == 0 && rz == 0)
      {
        countRotZeros++;
      }
      else
      {
        countRotZeros = 0;
      }
// check if the next state should be keys
#if (NUMKEYS > 0)
      if (memcmp(keyData, prevKeyData, HIDMAXBUTTONS / 8) != 0)
      // compare key data to previous key data
      {
        nextState = ST_SENDKEYS;
      }
      else
      {
        // go back to start
        nextState = ST_START;
      }
#else
      // if no keys are used, go to start state after rotations
      nextState = ST_START;
#endif
    }
    break;
#if (NUMKEYS > 0)
  case ST_SENDKEYS:
    // report the keys, if the 8 ms since the last report have past
    if (now - lastHIDsentRep >= HIDUPDATERATE_MS)
    {
      HID().SendReport(3, keyData, HIDMAXBUTTONS / 8);
      lastHIDsentRep += HIDUPDATERATE_MS;
      memcpy(prevKeyData, keyData, HIDMAXBUTTONS / 8); // copy actual keyData to previous keyData
      hasSentNewData = true;                           // return value
      nextState = ST_START;                            // go back to start
    }
    break;
#endif
  default:
    nextState = ST_START; // go back to start in error?!
    // send nothing if all data is zero
    break;
  }

  return hasSentNewData;
}

#if (NUMKEYS > 0)
// Takes the data in keys and sort them into the bits of keyData
// Which key from keyData should belong to which byte is defined in bitNumber = BUTTONLIST see config.h
void prepareKeyBytes(uint8_t *keys, uint8_t *keyData, int debug)
{
  for (int i = 0; i < HIDMAXBUTTONS / 8; i++) // init or empty this array
  {
    keyData[i] = 0;
  }

  for (int i = 0; i < NUMHIDKEYS; i++)
  {
    // check for every key if it is pressed
    if (keys[i])
    {
      // set the according bit in the data bytes
      // byte no.: bitNumber[i] / 8
      // bit no.:  bitNumber[i] modulo 8
      keyData[(bitNumber[i] / 8)] = (1 << (bitNumber[i] % 8));
      if (debug == 8)
      {
        // debug the key board outputs
        Serial.print("bitnumber: ");
        Serial.print(bitNumber[i]);
        Serial.print(" -> keyData[");
        Serial.print((bitNumber[i] / 8));
        Serial.print("] = ");
        Serial.print("0x");
        Serial.println(keyData[(bitNumber[i] / 8)], HEX);
      }
    }
  }
}
#endif