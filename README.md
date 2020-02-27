# Real Time System A1

This program will run on thie Viva TM4C1294XL.

For this assignment, we are asked to design, build and test an interrupt driven monitor which communicate to a VT-100 terminal through UART. User should be able to input command that set or read time and date. Also, user can turn off or set an alarm by giving delta time value.

The program has three layers: application layer, queue layer and UART layer. UART layer is the layer that receive and transmit data through VT-100 terminal. Also, UART layer can receive and handle interrupts. Queue layer is to store data which has received or ready to be transmitted. EnQueue and DeQueue function are the only two function that can be accessed by UART and application. Application layer is to process the received data which is stored in input queue. Also, application layer should send output data by enqueuing to output queue.