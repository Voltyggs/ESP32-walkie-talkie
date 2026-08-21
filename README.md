<h1>ESP TALK - the off grid solution to communication</h1>

ESP TALK is a walkie talkie based on the ESP32 and the ESPNOW protocol to provide a off gid communication medium

<img width="1406" height="756" alt="Screenshot 2026-08-15 175706" src="https://github.com/user-attachments/assets/6178901f-ad56-4ab8-8332-02e75cc9d33e" /><br>

<h3>This is the schematic of my walkie talkie. The project consists of an esp32, mic, speaker, audio amplifier + DAC, and dual power supplies <br>
The i2s Mic first transmits its audio signals to the esp32 when the PTT button is held. After, the esp32 transmits the audio packets to the 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF address. The nearby walkie talkies pick up the signal and play it through the speaker which is powered by an MAX98351 which also doubles as a DAC for the esp32 for a better audio</h3>

- Main Features:
  - Completely self dependent and off grid as it runs with ESPNOW
  - antenna for a better range
  - uses the 12 bit DAC of the MAX98351 rather than the esp32's 8 bit DAC

How to use:
This is a fairly simple project, after assembling it, You can edit the transmit aaddress or keep it as default (0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF). After that, just hold down teh push to talk button and transmit!
