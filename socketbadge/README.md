# DEFCON 24 Car Hacking Badge SocketCAN Compatibility

This is just a quic and dirty hack to allow use of the DEFCON 24 Car Hacking
Village badge to be used as a set of interfaces with SocketCAN. Current
functionality is very basic. In the future we hope to make the following
improvements:

 - Add the capability to set the CAN bus bitrates from the userspace app
   (currently the bitrates are hard-coded in the Pawn script)
 - Change the USB command header byte to be a dedicated type so that we
   doin't stomp the badge's ability to provide serial debug output.
 - Maybe add libcap support so that the userspace process doesn't need
   to run as root.

There are two parts to this project. First is the Pawn script that must 
be loaded onto the badge. This is contained in the file "can_socket.p".
This Pawn script was written by Nathan Hoch and slightly modified by 
me. Follow Nathan's excellent instructions in the SDK documentation
for how to build and install Pawn scripts on to the badge.

The second part is a Linux userspace process that uses libusb-1.0 to
talk to the badge over USB and then gateway traffic back and forth to 
two SocketCAN virtual CAN (vcan) interfaces. The requires the libusb-1.0
headers and libraries installed in order to build. On Ubuntu-derived 
systems, run the following to install the dependencies:

```
sudo apt install build-essential libusb-1.0-0-dev
```

To build the socketbadge binary, do the following:

```
cmake CmakeLists.txt
make
```

The resultant binary will be placed in the bin/ subdirectory.

To setup and run socketbadge, do the following:

```
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link add dev vcan1 type vcan
sudo ip link set vcan0 up
sudo ip link set vcan1 up
sudo socketbadge -0 vcan0 -1 vcan1
```

You will then be able to send/receive traffic to/from the badge by just using
the vcan0 and vcan1 interfaces as you would any other SocketCAN interface.

Happy Hacking!

Rob "Deker" Dekelbaum - deker@autoimmune.io
