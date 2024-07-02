# Space Navigator
This page contains informations about a 3Dconnexion SpaceNavigator, which has been observed via wireshark:

`sudo modprobe usbmon` 
`sudo wireshark`


Test on update rate of the spacemouse with wireshark and spacenavd on ubuntu. src 1.8.1

## Data transmitted
First package: (no translation seen here)
`01 00 00 00 00 00 00`

Second package: (rotation around "right down" = "second axis")
`02 00 00 e2 ff 00 00`

Pushing Button 1:
`03 01 00`

Releasing Button 1:
`03 00 00`

Pushing Button 2:
`03 02 00`

Release Button 2:
`03 00 00`

Pushing both buttons at the same time: 
`03 03 00`

If in spnavcfg the LED is turned on, the host sends to 1.8.2
`04 01`

Turning LED off:
`04 00`

## Timing of the data sent
Report Id 1 for translation is sent and 8 ms later Report Id 2 for rotation.
This leads to 125 packages per second with alternating report id.

After the last package with non-zero data, three double-packages are sent afterwards.


## HID Descriptor 
```
0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
0x09, 0x08,        // Usage (Multi-axis Controller)
0xA1, 0x01,        // Collection (Application)
0xA1, 0x00,        //   Collection (Physical)
0x85, 0x01,        //     Report ID (1)
0x16, 0xA2, 0xFE,  //     Logical Minimum (-350)
0x26, 0x5E, 0x01,  //     Logical Maximum (350)
0x36, 0x88, 0xFA,  //     Physical Minimum (-1400)
0x46, 0x78, 0x05,  //     Physical Maximum (1400)
0x55, 0x0C,        //     Unit Exponent (-4)
0x65, 0x11,        //     Unit (System: SI Linear, Length: Centimeter)
0x09, 0x30,        //     Usage (X)
0x09, 0x31,        //     Usage (Y)
0x09, 0x32,        //     Usage (Z)
0x75, 0x10,        //     Report Size (16)
0x95, 0x03,        //     Report Count (3)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0xA1, 0x00,        //   Collection (Physical)
0x85, 0x02,        //     Report ID (2)
0x09, 0x33,        //     Usage (Rx)
0x09, 0x34,        //     Usage (Ry)
0x09, 0x35,        //     Usage (Rz)
0x75, 0x10,        //     Report Size (16)
0x95, 0x03,        //     Report Count (3)
0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0xA1, 0x02,        //   Collection (Logical)
0x85, 0x03,        //     Report ID (3)
0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
0x05, 0x09,        //     Usage Page (Button)
0x19, 0x01,        //     Usage Minimum (0x01)
0x29, 0x02,        //     Usage Maximum (0x02)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x35, 0x00,        //     Physical Minimum (0)
0x45, 0x01,        //     Physical Maximum (1)
0x75, 0x01,        //     Report Size (1)
0x95, 0x02,        //     Report Count (2)
0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0x95, 0x0E,        //     Report Count (14)
0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0xA1, 0x02,        //   Collection (Logical)
0x85, 0x04,        //     Report ID (4)
0x05, 0x08,        //     Usage Page (LEDs)
0x09, 0x4B,        //     Usage (Generic Indicator)
0x15, 0x00,        //     Logical Minimum (0)
0x25, 0x01,        //     Logical Maximum (1)
0x95, 0x01,        //     Report Count (1)
0x75, 0x01,        //     Report Size (1)
0x91, 0x02,        //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x95, 0x01,        //     Report Count (1)
0x75, 0x07,        //     Report Size (7)
0x91, 0x03,        //     Output (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //   End Collection
0x06, 0x00, 0xFF,  //   Usage Page (Vendor Defined 0xFF00)
0x09, 0x01,        //   Usage (0x01)
0xA1, 0x02,        //   Collection (Logical)
0x15, 0x80,        //     Logical Minimum (-128)
0x25, 0x7F,        //     Logical Maximum (127)
0x75, 0x08,        //     Report Size (8)
0x09, 0x3A,        //     Usage (0x3A)
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x05,        //       Report ID (5)
0x09, 0x20,        //       Usage (0x20)
0x95, 0x01,        //       Report Count (1)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x06,        //       Report ID (6)
0x09, 0x21,        //       Usage (0x21)
0x95, 0x01,        //       Report Count (1)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x07,        //       Report ID (7)
0x09, 0x22,        //       Usage (0x22)
0x95, 0x01,        //       Report Count (1)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x08,        //       Report ID (8)
0x09, 0x23,        //       Usage (0x23)
0x95, 0x07,        //       Report Count (7)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x09,        //       Report ID (9)
0x09, 0x24,        //       Usage (0x24)
0x95, 0x07,        //       Report Count (7)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x0A,        //       Report ID (10)
0x09, 0x25,        //       Usage (0x25)
0x95, 0x07,        //       Report Count (7)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x0B,        //       Report ID (11)
0x09, 0x26,        //       Usage (0x26)
0x95, 0x01,        //       Report Count (1)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xA1, 0x02,        //     Collection (Logical)
0x85, 0x13,        //       Report ID (19)
0x09, 0x2E,        //       Usage (0x2E)
0x95, 0x01,        //       Report Count (1)
0xB1, 0x02,        //       Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0xC0,              //     End Collection
0xC0,              //   End Collection
0xC0,              // End Collection

// 217 bytes



0000   05 01 09 08 a1 01 a1 00 85 01 16 a2 fe 26 5e 01   .............&^.
0010   36 88 fa 46 78 05 55 0c 65 11 09 30 09 31 09 32   6..Fx.U.e..0.1.2
0020   75 10 95 03 81 06 c0 a1 00 85 02 09 33 09 34 09   u...........3.4.
0030   35 75 10 95 03 81 06 c0 a1 02 85 03 05 01 05 09   5u..............
0040   19 01 29 02 15 00 25 01 35 00 45 01 75 01 95 02   ..)...%.5.E.u...
0050   81 02 95 0e 81 03 c0 a1 02 85 04 05 08 09 4b 15   ..............K.
0060   00 25 01 95 01 75 01 91 02 95 01 75 07 91 03 c0   .%...u.....u....
0070   06 00 ff 09 01 a1 02 15 80 25 7f 75 08 09 3a a1   .........%.u..:.
0080   02 85 05 09 20 95 01 b1 02 c0 a1 02 85 06 09 21   .... ..........!
0090   95 01 b1 02 c0 a1 02 85 07 09 22 95 01 b1 02 c0   ..........".....
00a0   a1 02 85 08 09 23 95 07 b1 02 c0 a1 02 85 09 09   .....#..........
00b0   24 95 07 b1 02 c0 a1 02 85 0a 09 25 95 07 b1 02   $..........%....
00c0   c0 a1 02 85 0b 09 26 95 01 b1 02 c0 a1 02 85 13   ......&.........
00d0   09 2e 95 01 b1 02 c0 c0 c0                        .........
```