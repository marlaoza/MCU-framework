#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#include <Arduino.h>

// --- DISPLAY SETTINGS (GC9A01) ---
#define TFT_RES      4
#define TFT_CS       5
#define TFT_DC       15
#define TFT_MOSI     23
#define TFT_SCLK     18
#define TFT_BL       22

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 240
#define SCREEN_RADIUS 120
#define SCREEN_CENTER 120

// --- BUTTONS ---
#define BTN_26      26    
#define BTN_25      25    
#define BTN_35      35    
#define BTN_27      27    
#define BTN_16      16   
#define BTN_17      17   

// --- TIPO DE INPUT ---
#define NAV_UP BTN_26
#define NAV_DOWN BTN_25
#define NAV_LEFT BTN_35
#define NAV_RIGHT BTN_27
#define NAV_CONFIRM BTN_16
#define NAV_CANCEL BTN_17


// --- SYSTEM SETTINGS ---
#define TARGET_FPS   60
#define FRAME_DELAY  (1000 / TARGET_FPS)

// --- LORA SX1268 (SPI) ---

#define LORA_SPI_SCK  14  
#define LORA_SPI_MOSI 13  
#define LORA_SPI_MISO 12  

#define LORA_CS        2  
#define LORA_RST      32  
#define LORA_DIO1     33  
#define LORA_BUSY     34  

//Battery
#define BATTERY_PIN 36

// --- STORAGE (SD CARD) ---
#define SD_CS        2

// --- SENSORS (I2C) ---
#define MPU_SDA      21
#define MPU_SCL      22
#define MPU_TRIG     19
// --- API URLS ---
#define API_BASE_URL "https://api-mochilume-acd9htg9ghhacghy.brazilsouth-01.azurewebsites.net"
// Global
#define HEALTH_ENDPOINT       API_BASE_URL "/health"
// Auth
#define AUTH_REGISTER_ENDPOINT API_BASE_URL "/api/Auth/register"
#define AUTH_LOGIN_ENDPOINT    API_BASE_URL "/api/Auth/login"
// Save System
#define SAVE_UPLOAD_ENDPOINT   API_BASE_URL "/api/Save/upload"
// Para o download, você pode concatenar o username direto na string se preferir, 
// ou usar como base para um sprintf/snprintf formatado:
#define SAVE_DOWNLOAD_BASE_URL API_BASE_URL "/api/Save/"
#define SAVE_DOWNLOAD_SUFFIX   "/download"

//PATH DE ARQUIVO DE SAVE
#define PLAYER_SAVE_PATH "/player.sav"


#endif