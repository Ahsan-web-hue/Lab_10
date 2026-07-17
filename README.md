# Lab_10
Timer Base STM32 Programming with STM32F103C8T6 Blue Pill C++
**Software Requirements**_
STM32CubeIDE
STM32CubeMX (Integrated with STM32CubeIDE)
STM32 HAL Drivers
GNU Arm Embedded Toolchain
**STM32CubeMX Configuration**
Create a new project for STM32F103C8Tx.
Enable a general-purpose timer (e.g., TIM2 or TIM3).
Configure the timer in Time Base mode.
Set the Prescaler and Auto Reload Register (ARR) according to the required timing interval.
Enable the timer interrupt if interrupt-based timing is required.
Configure the system clock.
Generate the project using C++.
Build and Flash
Open the project in STM32CubeIDE.
Build the project (Ctrl + B).
Connect the ST-Link programmer.
Flash the firmware to the STM32.
Start debugging and observe the timer operation.
**Repository Structure**
├── Core
│   ├── Inc
│   └── Src
├── Drivers
├── .ioc
├── README.md
└── LICENSE

If you have any questions, suggestions, or encounter any issues while using this project, feel free to reach out.
**Email: ahsanbasharatali38@gmail.com**
I'll be happy to help and will respond as soon as possible.
