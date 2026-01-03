#include <ESP8266WiFi.h>
#include <espnow.h>

// 板载 LED (调试用，收到数据闪一下)
#define LED_PIN 2

// === 1. 数据结构 (必须与发送端一致) ===
typedef struct struct_message {
  int d1;
  int d2;
  int d3;
  int d4;
} struct_message;

struct_message incomingData;

// === 2. 接收回调函数 (核心修改在这里) ===
void OnDataRecv(uint8_t * mac, uint8_t *incomingDataPtr, uint8_t len) {
  // 1. 把无线数据存入结构体
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  
  // 2. 调试反馈：让板载灯闪一下，代表收到数了
  digitalWrite(LED_PIN, LOW); 
  
  // === 3. 【关键】打包并发送给 STM32 ===
  uint8_t serialPacket[6];

  serialPacket[0] = 0xAA; // 帧头
  
  // 提取数据：把 int (0或1) 转成 byte 发送
  serialPacket[1] = (uint8_t)incomingData.d1;
  serialPacket[2] = (uint8_t)incomingData.d2;
  serialPacket[3] = (uint8_t)incomingData.d3;
  serialPacket[4] = (uint8_t)incomingData.d4;
  
  serialPacket[5] = 0xFF; // 帧尾

  // 通过串口直接发给 STM32
  Serial.write(serialPacket, 6);

  // 闪完灯关掉
  // (稍微延时一点点让肉眼能看见闪烁，如果追求极速通信可以去掉 delay)
  // delay(5); 
  digitalWrite(LED_PIN, HIGH);
}

void setup() {
  // 这里的波特率必须和 STM32 的设置一模一样！建议 115200
  Serial.begin(115200);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // === ESP-NOW 初始化 ===
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // 啥也不用做，全靠回调函数驱动
}