#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <driver/i2s.h>

#define PTT_PIN 4
#define MIC_SCK_PIN 14  
#define MIC_WS_PIN 33
#define MIC_SD_PIN 32
#define AMP_BCLK_PIN 26  
#define AMP_LRC_PIN 27  
#define AMP_DIN_PIN 25

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 200 

uint8_t txBuffer[BUFFER_SIZE];
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

bool isTransmitting = false;




void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  if (digitalRead(PTT_PIN) == HIGH) {
    size_t bytesWritten = 0;
    i2s_write(I2S_NUM_1, incomingData, len, &bytesWritten, portMAX_DELAY);
  }
}

void initI2S() {
  i2s_config_t micConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 100,
    .use_apll = false
  };

  i2s_pin_config_t micPins = {
    .bck_io_num = MIC_SCK_PIN,
    .ws_io_num = MIC_WS_PIN,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = MIC_SD_PIN
  };

  i2s_driver_install(I2S_NUM_0, &micConfig, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &micPins);

  i2s_config_t ampConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 100,
    .use_apll = false
  };

  i2s_pin_config_t ampPins = {
    .bck_io_num = AMP_BCLK_PIN,
    .ws_io_num = AMP_LRC_PIN,
    .data_out_num = AMP_DIN_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM_1, &ampConfig, 0, NULL);
  i2s_set_pin(I2S_NUM_1, &ampPins);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  pinMode(PTT_PIN, INPUT_PULLUP);
  
  initI2S();
  Serial.println("Audio ok");

  WiFi.mode(WIFI_STA);
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);

  if (esp_now_init() != ESP_OK) {
    Serial.println("espnoW error.");
    return;
  }
  
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to register broadcast channel");
    return;
  }
}

void loop() {
  bool buttonPressed = (digitalRead(PTT_PIN) == LOW);
  if (buttonPressed && !isTransmitting) {
    Serial.println("Mic open");
    isTransmitting = true;
  } 
  else if (!buttonPressed && isTransmitting) {
    Serial.println("mic closed");
    isTransmitting = false;
  }

  if (buttonPressed) {
    size_t bytesRead = 0;

    i2s_read(I2S_NUM_0, txBuffer, BUFFER_SIZE, &bytesRead, portMAX_DELAY);
    
    if (bytesRead > 0) {
      esp_now_send(broadcastAddress, txBuffer, bytesRead);
    }
  }
}