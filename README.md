# ||AFKS||

## ⚡Home Energy Consumption Monitor with LCD Display

Welcome all! In this repository, you will hopefully find everything you need to carry out the **Home Energy Consumption Monitor** project. 

This design involves the implementation of an **STM32F103C8T6 BluePill MCU** to monitor and display electrical parameters such as:
- Current
- Voltage
- Power factor
- Energy consumption in kWh  

The information is shown on a display, allowing real-time readings of measurements. The system also includes an **alarm (LED)** to warn about critical values and to protect against overvoltage and overcurrent.

---

## 🚀Key Features

### Energy Consumption Monitoring:
- Current and voltage measuring through sensors connected to the BluePill's ADC.
- Real-time energy consumption calculation in kWh.

### Power Factor:
- Phase shift measurement between the current and voltage signals.
- Power factor calculation.

### Data Transfer:
- Real-time gathered data displayed on an LCD 16x4 using I2C.

### Alarm System:
- LED indicator triggers when Power Factor or Energy Consumption values exceed recommended thresholds.

### Energy Usage Optimization:
- Provide users with sufficient information to make informed decisions and reduce energy consumption at home.

---

## :hammer_and_wrench:Hardware

- **MCU**: STM32F103C8T6 BluePill  
- **SCT013**: Current sensor  
- **Voltage Transformer**  
- **LCD**: 16x4 display with PCF8574 I2C Driver for communication with the MCU  
- **Optocoupler**: For zero-crossing detection, used in power factor calculation  

---

## :desktop_computer:Software

- **PlatformIO** / **STM32CubeIDE**  
- **Libraries**:  
  - `libopencm3` (rcc.h, gpio.h, adc.h, i2c.h, nvic.h, systick.h, timer.h, dma.h)  
  - LCD library (`lcd.h`)  

---

## :electric_plug:Schematic Diagram

![Schematic Diagram](https://github.com/user-attachments/assets/85041b3e-340c-4535-aa57-fe148c72804a)
Arrangement of the components used in the project with their respective connections.

---

## :trophy:Results

The project will include:
- **Project exposition**: A complete explanation of the code and a hardware overview.  
- **Video demonstration**: [https://youtu.be/-5vvS6dCAZk?si=XjdbSiEEBUvCcszg](#)  
