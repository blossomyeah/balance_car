#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Adafruit_NeoPixel.h>

// ==========================================
// 1. 接收端的身份证号 (MAC 地址) - 已填好
// ==========================================
uint8_t broadcastAddress[] = {0x68, 0xC6, 0x3A, 0xF6, 0xD8, 0x86};

// === 2. 硬件引脚定义 ===
const int PIN_D1 = 5;  // GPIO 5 (控制开关)
const int PIN_D2 = 4;  // GPIO 4
const int PIN_D3 = 0;  // GPIO 0
const int PIN_D4 = 2;  // GPIO 2

// WS2812 灯珠配置
#define PIN_LED    12
#define NUMPIXELS  1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);

// === 3. 数据包结构 (必须与接收端一致) ===
typedef struct struct_message {
  int d1;
  int d2;
  int d3;
  int d4;
} struct_message;

struct_message myData; // 创建数据包

// 发送结果回调函数 (可选调试用)
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  // Serial.print("发送状态: ");
  // Serial.println(sendStatus == 0 ? "成功" : "失败");
}

void setup() {
  Serial.begin(115200);

  // 初始化灯珠
  pixels.begin();
  pixels.setBrightness(25);
  pixels.show(); // 默认灭

  // 初始化按键 (上拉模式: 悬空=1, 接地=0)
  pinMode(PIN_D1, INPUT_PULLUP);
  pinMode(PIN_D2, INPUT_PULLUP);
  pinMode(PIN_D3, INPUT_PULLUP);
  pinMode(PIN_D4, INPUT_PULLUP);

  // === ESP-NOW 初始化 ===
  WiFi.mode(WIFI_STA); // 必须是 Station 模式
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW 初始化失败");
    return;
  }

  // 设置角色为控制器
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // 注册回调
  esp_now_register_send_cb(OnDataSent);
  
  // 添加接收端配对
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  Serial.println("遥控器已就绪，目标 MAC: 68:C6:3A:F6:D8:86");
}

void loop() {
  // 1. 读取引脚状态
  myData.d1 = digitalRead(PIN_D1);
  myData.d2 = digitalRead(PIN_D2);
  myData.d3 = digitalRead(PIN_D3);
  myData.d4 = digitalRead(PIN_D4);

  // 2. 本地灯光反馈 (D1接地变红)
  if (myData.d1 == HIGH) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // 绿 (悬空)
  } else {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // 红 (接地)
  }
  pixels.show();

  // 3. 无线发送数据
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  delay(50); // 发送间隔 50ms
}