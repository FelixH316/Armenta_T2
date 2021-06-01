/**
 * @file    main.c
 * @brief   T2 - RGB LED color change with switches on FRDM-K64F
 * @author	Félix Armenta A - IECA PADTS 3
 * @date	Jun 1st 2021
 */

/* TODO: insert other include files here. */
#include <stdint.h>
#include <stdbool.h>
#include "fsl_clock.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_common.h"

/* TODO: insert other definitions and declarations here. */
#define RLED 22u
#define GLED 26u
#define BLED 21u
#define SW2 6u
#define SW3 4u
#define CORE_FREQ 21000000u
#define DELAY 200000u
#define WHITE 99u
#define LAST 4u
#define FIRST 0u

gpio_pin_config_t led_config = {
		kGPIO_DigitalOutput,	// kGPIO_DigitalInput
		1,						// Valor inicial si se configura como Output
};

gpio_pin_config_t sw_config = {
		kGPIO_DigitalInput,
		0,
};

const port_pin_config_t input_config= {
		kPORT_PullUp,
		kPORT_FastSlewRate,
		kPORT_OpenDrainDisable,
		kPORT_PassiveFilterEnable,
		kPORT_HighDriveStrength,
		kPORT_MuxAsGpio,
		kPORT_UnlockRegister,
};

void RGB_LED (uint8_t color);

int main(void) {
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortE);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortA);

	// LEDs as GPIO
	PORT_SetPinMux(PORTB, RLED, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, GLED, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, BLED, kPORT_MuxAsGpio);

	// SWs config
	PORT_SetPinConfig(PORTC, SW2, &input_config);
	PORT_SetPinConfig(PORTA, SW3, &input_config);

	// LEDs as outputs and start value
	GPIO_PinInit(GPIOB, RLED, &led_config);
	GPIO_PinInit(GPIOE, GLED, &led_config);
	GPIO_PinInit(GPIOB, BLED, &led_config);

	// SWs as inputs
	GPIO_PinInit(GPIOC, SW2, &sw_config);
	GPIO_PinInit(GPIOA, SW3, &sw_config);

	uint8_t currentLED = 0;
	bool estadoSW2 = false;
	bool flagSW2 = false;
	bool estadoSW3 = false;
	bool flagSW3 = false;
	bool flagWhite = true;

	RGB_LED(currentLED);

    while(1) {
    	estadoSW2 = GPIO_PinRead(GPIOC, SW2);
    	estadoSW3 = GPIO_PinRead(GPIOA, SW3);

    	// WHITE AND FLAGS
    	if ((!estadoSW2) && (!estadoSW3)){
    		currentLED = WHITE;
    		flagWhite = 0;
    		flagSW2 = 0;
    		flagSW3 = 0;
    		SDK_DelayAtLeastUs(DELAY, CORE_FREQ);
    		estadoSW2 = 1;
    		estadoSW3 = 1;
    	}
    	else if (!estadoSW2){
    		flagSW2 = true;
    		flagWhite = true;
    	}
    	else if (!estadoSW3){
    		flagSW3 = true;
    		flagWhite = true;
    	}

    	if (flagWhite){
    		// INCREASING COUNT
    		if (flagSW3 && estadoSW3){
    			if (currentLED < LAST){
    				currentLED++;
    		    }
    		    else{
    		    	currentLED = FIRST;
    		    }
    		    flagSW3 = false;
    		}

    		// DECREASING COUNT
    		else if (flagSW2 && estadoSW2){
    			if (currentLED == WHITE){
    				currentLED = LAST;
    			}
    			else if (currentLED > FIRST){
    				currentLED--;
    			}
    			else{
    				currentLED = LAST;
    		    }
    		    flagSW2 = false;
    		}
    	}
    	RGB_LED(currentLED);
    }
    return 0 ;
}

void RGB_LED (uint8_t color){
	switch (color){
	case 0:
		// GREEN
		GPIO_PortSet(GPIOB, 1u << RLED);
		GPIO_PortClear(GPIOE, 1u << GLED);
		GPIO_PortSet(GPIOB, 1u << BLED);
		break;
	case 1:
		// BLUE
		GPIO_PortSet(GPIOB, 1u << RLED);
		GPIO_PortSet(GPIOE, 1u << GLED);
		GPIO_PortClear(GPIOB, 1u << BLED);
		break;
	case 2:
		// PURPLE
		GPIO_PortClear(GPIOB, 1u << RLED);
		GPIO_PortSet(GPIOE, 1u << GLED);
		GPIO_PortClear(GPIOB, 1u << BLED);
		break;
	case 3:
		// RED
		GPIO_PortClear(GPIOB, 1u << RLED);
		GPIO_PortSet(GPIOE, 1u << GLED);
		GPIO_PortSet(GPIOB, 1u << BLED);
		break;
	case 4:
		// YELLOW
		GPIO_PortClear(GPIOB, 1u << RLED);
		GPIO_PortClear(GPIOE, 1u << GLED);
		GPIO_PortSet(GPIOB, 1u << BLED);
		break;
	default:
		// WHITE
		GPIO_PortClear(GPIOB, 1u << RLED);
		GPIO_PortClear(GPIOE, 1u << GLED);
		GPIO_PortClear(GPIOB, 1u << BLED);
	}
}

