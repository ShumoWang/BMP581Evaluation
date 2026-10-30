# BMP581 Pressure and Altitude Evaluation

This project evaluates the Bosch BMP581 pressure sensor for low-power altitude tracking, with asset tracking applications in mind. Arduino was used to collect pressure readings, configure sensor output data rate (ODR) and oversampling ratio (OSR), and convert pressure to altitude. A Nordic PPK2 measured sensor power consumption.

## Work Performed

- Connected a SparkFun BMP581 breakout to an Arduino MKR Zero over I2C and recorded pressure data at different ODR and OSR settings.
- Characterized pressure noise and sensor power using the PPK2, including interrupt-driven readings.
- Used Jupyter notebooks to analyze the collected readings and compare OSR configurations.
- Implemented altitude calculations and tested relative altitude using two BMP581 sensors.

## Results

- In a 1,000-reading test (NORMAL mode, 240 Hz ODR, 100 readings/s), detrended pressure noise had a standard deviation of **0.586 Pa RMS** and a 99% interval of approximately **-1.51 to +1.51 Pa**. The report notes this is below the typical 0.95 Pa RMS datasheet value.
- OSR improved pressure precision, but higher OSR settings showed occasional abnormal readings, particularly at 64x and 128x. The report recommends **OSR 16x** as a precision-versus-charge trade-off.
- At 30 Hz ODR and 1x OSR, one measurement cycle was estimated to consume **61.71 µJ**, including configuration, measurement, and the Arduino read request. For repeated 1 Hz readings, the report estimates **9.9 µW** operating power plus configuration energy, with ODR set close to the required reading rate.
- The two-sensor relative-altitude test estimated a first-floor height of **4.4 m**, compared with a stair-based estimate of **4.14 m** (6.3% difference). Quick tests indicated changes could be resolved to about **±0.5 m** over a short period.

Pressure-based altitude depends on a stable reference pressure. Ambient pressure changes caused drift, so the relative-altitude result was only reliable over a short period. The report recommends testing in a pressure-controlled chamber and investigating the high-OSR anomalies and power measurements further.

A more detailed report can be found [here](https://github.com/ShumoWang/BMP581Evaluation/blob/main/final_report.pdf)

## Repository Contents

- `AltitudeMeasurementByPressure/` and `ArduinoAltitudeTesting/`: Arduino sketches for pressure-to-altitude and sensor reading tests.
- `Example5_Oversampling/`: Arduino sketch demonstrating OSR, ODR, and data-ready interrupts.
- `DataAnalysis/`: Pressure logs and notebooks for normal-mode and OSR analysis.
- `*.ppk2`: Nordic PPK2 power measurement captures for different ODR/OSR configurations.

## Hardware and Setup

- Arduino MKR Zero
- SparkFun BMP581 pressure sensor breakout
- Nordic Power Profiler Kit II (PPK2) for power measurements
- Arduino IDE with the SparkFun BMP581 Arduino library

Open the desired `.ino` sketch in Arduino IDE, select the connected board and port, then upload it. The oversampling example uses a data-ready interrupt connected to pin `A2`. The sketches print measurements over Serial at **115200 baud**.