# CAR_BLACK_SYSTEM
We are advancing at a never-before-seen rate in this rapidly changing field of technology.  Even the smallest things now have a life of their own thanks to embedded systems, artificial intelligence, and the Internet of Things. This has turned the world into a magical place where our smartphones are the wand.  A wide range of industrial sectors, including commercial, manufacturing, automotive, defense, aerospace, and aeronautics, use embedded devices.  The Car Black Box, my project, explores one of these innovative technological developments.
 A device that carefully records and monitors every occurrence on an airplane, from speed and altitude to pilot discussions, is referred to as a "black box" in the aviation industry.  Comparably, the Car Black Box records events that occur in a car, like speed, gear changes, and crashes, and displays a menu screen for the user to interact with.  Users can download saved data to a laptop via UART connection using this menu screen, which is protected by a passcode that can be changed.

▶ How to Run the Project
Open the CAR_BLACK_BOX.X project in MPLAB X IDE.

Connect PICSimLab and select appropriate board (e.g., PIC16F877a).

Compile the project using the XC8 compiler.

Load the generated .hex file into PICSimLab.

Use:

A potentiometer (or analog control) to simulate vehicle speed.

A button to simulate accident (external interrupt).

Observe data on LCD Display
