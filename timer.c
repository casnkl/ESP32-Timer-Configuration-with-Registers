#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/timer_group_reg.h"
#include "timer.h"
#include "driver/gpio.h"
#include "esp_task_wdt.h"

#define TIMG0_BASE (DR_REG_TIMERGROUP0_BASE)
#define LED_GPIO_NUM 14

// Timer 0 registers of Timer Group 0 (TIMG0)
volatile uint32_t *TIMG0_T0CONFIG_REG      = (volatile uint32_t *)(TIMG0_BASE);       // Control register
volatile uint32_t *TIMG0_T0ALARMLO_REG     = (volatile uint32_t *)(TIMG0_BASE+0x10);  // Alarm value low
volatile uint32_t *TIMG0_T0ALARMHI_REG     = (volatile uint32_t *)(TIMG0_BASE+0x14);  // Alarm value high
volatile uint32_t *TIMG0_T0LOADLO_REG      = (volatile uint32_t *)(TIMG0_BASE+0x18);  // Auto-reload value low
volatile uint32_t *TIMG0_T0LOADHI_REG      = (volatile uint32_t *)(TIMG0_BASE+0x1C);  // Auto-reload value high
volatile uint32_t *TIMG0_T0LOAD_REG        = (volatile uint32_t *)(TIMG0_BASE+0x20);  // Load register to apply values

bool state = 0;

void timerInit(){

    // 1. Stop the timer before configuring
    *TIMG0_T0CONFIG_REG = 0;
    
    // 2. Set the alarm value (when the timer will trigger)
    *TIMG0_T0ALARMLO_REG = 0x1312D00; 
    *TIMG0_T0ALARMHI_REG = 0;

    // 3. Set the initial/load value of the timer
    *TIMG0_T0LOADLO_REG = 0;
    *TIMG0_T0LOADHI_REG = 0;
    *TIMG0_T0LOAD_REG = 1; // Apply the load values
    
    // 4. Configure the timer:
    // - Divider = 2
    // - Auto-reload enabled
    // - Count-up mode
    // - Enable timer
    *TIMG0_T0CONFIG_REG = (2 << 13) |  // Divider
                          (1 << 29) |  // Auto-reload
                          (1 << 30) |  // Count-up
                          (1 << 31);   // Timer enable
}

void initGPIO(){
    gpio_reset_pin(LED_GPIO_NUM);                  // Reset GPIO to default state
    gpio_set_direction(LED_GPIO_NUM, GPIO_MODE_OUTPUT); // Set as output
    gpio_set_level(LED_GPIO_NUM, 0);               // Initialize LED off
}

void start_timer(){
    esp_task_wdt_deinit();       // Disable the watchdog to prevent resets
    timerInit();                 // Initialize and start the timer
    *TIMG0_T0CONFIG_REG |= (1 << 10); // Enable timer alarm

    while(1){
        // Wait for the timer alarm flag
        while((*TIMG0_T0CONFIG_REG & (1 << 10))){
        }

        // Toggle the LED state
        gpio_set_level(LED_GPIO_NUM, !gpio_get_level(LED_GPIO_NUM));
        
        // Clear alarm flag by re-enabling it
        *TIMG0_T0CONFIG_REG |= (1 << 10);
        printf("LED toggled\n");
    }
}
