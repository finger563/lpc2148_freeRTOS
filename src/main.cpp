/*******************************************************************************
 *                                                                              *
 * LOGOMATIC MULTITASKING BLINKY TEST                                           *
 *                                                                              *
 * AUTHOR: Jean-Sebastien Stoezel                                               *
 *         js.stoezel@gmail.com                                                 *
 *                                                                              *
 *******************************************************************************/

/*******************************************************************************
 *                                                                              *
 * INCLUDES                                                                     *
 *                                                                              *
 *******************************************************************************/

#include <stdint.h>
#include "Logomatic_V2.hpp"

void abort(void)
{
  while (1 == 1)
    {}
} // abort

#include "FreeRTOS.h"
#include "task.h"

#define TASK_STACK_SIZE             300
#define LED_TASK_PRIORITY           4

typedef struct
{
  uint32_t frequency;            // Blinking frequency
} sTaskParams_t, *spTaskParams_t;


char led_state = led_off;
xTaskHandle xLEDTaskHandle;

static void LEDTask ( void * pvParameters );
void set_leds(char num, char on_off);



sTaskParams_t CountingArgs = {
  20    // Task Execution Frequency
};

static void LEDTask( void * pvParameters ) {
  sTaskParams_t *  p_Args = reinterpret_cast<sTaskParams_t *>(pvParameters);
    
  while (true) {
    set_leds(led_all,led_state);
    led_state = !led_state;
    vTaskDelay(configTICK_RATE_HZ / p_Args->frequency);
  }
}

void set_leds(char num, char on_off)
{
  switch(num)
    {
    case 0:
      {
	if(on_off)
	  P0CLR = led0;
	else
	  P0SET = led0;
      }break;
    case 1:
      {
	if(on_off)
	  P1CLR = led1;
	else
	  P1SET = led1;
      }break;
    case 2:
      {
	if(on_off)
	  P1CLR = led2;
	else
	  P1SET = led2;
      }break;
    case 3:
      {
	if(on_off)
	  P0CLR = led3;
	else
	  P0SET = led3;
      }break;
    case led_all:
      {
	if(on_off)
	  {
	    P0CLR = led0;
	    P1CLR = led1;
	    P1CLR = led2;
	    P0CLR = led3;
	  }
	else
	  {
	    P0SET = led0;
	    P1SET = led1;
	    P1SET = led2;
	    P0SET = led3;
	  }
      }break;
    default:
      {
	P0SET = led0;
	P1SET = led1;
	P1SET = led2;
	P0SET = led3;
      }
      break;
    }
}

extern "C" {
  void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
  {
    volatile signed char *name;
    volatile xTaskHandle *pxT;

    name = pcTaskName;
    pxT  = pxTask;
    while(1) {
      set_leds(led0, led_off);
    }
  }

  void vApplicationMallocFailedHook(void)
  {
    while(1) {
      set_leds(led1, led_off);
    }
  }
}

static void
initTimer0(void (*pISR)(void))          //, void (*pCallback)()
{       
  //initialize VIC for Timer0 interrupts
  VICIntSelect &= ~0x10;       //Timer0 interrupt is assigned to IRQ (not FIQ)
  VICVectCntl1  = 0x24;        //enable vector interrupt for timer0
  VICVectAddr1  = (unsigned int)pISR;  //register ISR address
  VICIntEnable  |= 0x10;        //enable timer0 interrupt       
        
  //initialize and start Timer0
  T0TCR = 0x00000000;                           //disable and reset Timer0
  T0PR  = 0x00000000;
  T0PC  = 0x00000000;                           //no prescale of clock
  T0MR0 = 6000000;                              // 100 ms interrupts on 60 MHz clk
  T0IR  = 0x000000ff;                           //reset all flags before enable IRQs
  T0MCR = 0x00000003;                           //reset counter and generate IRQ on MR0 match
  T0CCR = 0x00000000;
  T0TCR = 0x00000001;                           //start Timer0
}

static void
timer0ISR(void)
{
  set_leds(led_all, led_state);
  led_state = !led_state;

  T0IR        = 0xff;        //reset all IRQ flags
  VICVectAddr = 0x00;        //dummy write to VIC to signal end of interrupt
}

/*******************************************************************************
 *                                                                              *
 * main                                                                         *
 *                                                                              *
 *******************************************************************************/
int main( void )
{   
  LogomaticV2Init();

  set_leds(led_all, led_on);

#if 1
  initTimer0(timer0ISR);
#else
  xTaskCreate(LEDTask,
	      reinterpret_cast<const char *>("LEDTask"),
	      TASK_STACK_SIZE,
	      &CountingArgs,
	      LED_TASK_PRIORITY,
	      &xLEDTaskHandle
	      );

  vTaskStartScheduler();
#endif   
  set_leds(led3, led_off);

  return 0;
} // main
