Hiya, this is a project submission for stardance

This is a project where I made an esp32 walkie talkie

Commercial walkie talkies rely on crowded analog RF channels, while standard microcontroller voice applications usually suffer from heavy Wi-Fi latency and complex network setups. This project eliminates by streaming uncompressed digital audio directly between microcontrollers over raw radio frames.

Because the system uses a universal broadcast MAC address (FF:FF:FF:FF:FF:FF), it operates as a decentralized system. Any number of walkie talkies  loaded with the same code can join the network without manual pairing or central router infrastructure.

The project mainly features:
ESP3-WROOM-32U
INMP441 Mic
MAX98357 Amp

Hardware Architecture & Audio Pipeline:
The voice hardware bypasses noisy analog microcontrollers entirely by using a fully digital I2S audio chain. Voice input is captured using an INMP441 MEMS microphone, which streams 16 bit PCM audio directly into the ESP32’s internal I2S system at 16,000 Hz. Audio playback is handled by a MAX98357A Class-D amplifier and a small 3W speaker.

To keep memory usage light , the continuous audio stream is sliced into 200-byte frame buffers. When the Push-To-Talk button is held, these audio packets fire continuously over the airwaves. When the button is released, the local microphone pauses and the board listens for incoming broadcasts.


Power Delivery:
Transmitting over ESP-NOW at maximum output causes sudden current spikes, often exceeding 500mA as the power amplifier fires up. Operating directly off a discharging 18650 lithium-ion cell causes severe voltage sags that lead to system brownouts and random micro-reboots.

To guarantee stability across the entire discharge cycle of the batteries, an MT3608 boost converter steps the unsteady battery voltage up to a steady 5V. A capacitor placed across the main 5V rail acts as a voltage stabilizer.


Hardware Setup:
Always adjust the MT3608 boost converter output to exactly 5.0V using a multimeter before wiring power to the ESP32 or audio amp.

Never power on the ESP32 with maximum transmission power enabled unless an external 2.4GHz antenna is connected to the u.FL port, as missing loads can cause thermal degradation over time. Keep antennas oriented vertically for optimal signal spread.

<img width="1406" height="756" alt="Screenshot 2026-08-15 175706" src="https://github.com/user-attachments/assets/b065648d-adce-4646-8913-6e061e6e9835" />
Here is the schematic. Note that there is no pcb diagram as I plan to make this on a perfboard.
