#include <string.h>
#include "usbh_usr.h"
#include "ff.h"      
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;

uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;
uint8_t filenameString[15]  = {0};

FATFS fatfs;
FIL file;

uint8_t line_idx = 0;   
uint32_t BytesRead = 0;
int f_o=0;


USBH_Usr_cb_TypeDef USR_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
    
};

typedef enum 
{  
  BUTTON_USER = 0,
} Button_TypeDef;


typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;    

#define LEDn                             2
#define BUTTONn                          1  

#define LED3_PIN                         GPIO_Pin_13
#define LED3_GPIO_PORT                   GPIOG
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOG  

#define LED4_PIN                         GPIO_Pin_14
#define LED4_GPIO_PORT                   GPIOG
#define LED4_GPIO_CLK                    RCC_AHB1Periph_GPIOG  

#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define USER_BUTTON_EXTI_LINE          EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn          EXTI0_IRQn 

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED3_GPIO_PORT, LED4_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED3_PIN, LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED3_GPIO_CLK, LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {USER_BUTTON_GPIO_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {USER_BUTTON_PIN}; 
const uint32_t BUTTON_CLK[BUTTONn] = {USER_BUTTON_GPIO_CLK};
const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {USER_BUTTON_EXTI_LINE};
const uint8_t BUTTON_PORT_SOURCE[BUTTONn] = {USER_BUTTON_EXTI_PORT_SOURCE};
const uint8_t BUTTON_PIN_SOURCE[BUTTONn] = {USER_BUTTON_EXTI_PIN_SOURCE}; 
const uint8_t BUTTON_IRQn[BUTTONn] = {USER_BUTTON_EXTI_IRQn};


void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

  EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure); 

}

void USBH_USR_DeviceNotSupported(void)
{
} 

void USBH_USR_UnrecoveredError (void)
{
}


void USBH_USR_Init(void)
{
      STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

}


void USBH_USR_DeviceAttached(void)
{

}




void USBH_USR_DeviceDisconnected (void)
{

}

void USBH_USR_ResetDevice(void)
{
}


void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
}


void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
 // USBH_DevDesc_TypeDef *hs;
 // hs = DeviceDesc;  
  
}

void USBH_USR_DeviceAddressAssigned(void)
{
  
}


void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
 // USBH_InterfaceDesc_TypeDef *id;
  
 // id = itfDesc;  
     
}


void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
}


void USBH_USR_Product_String(void *ProductString)
{
}


void USBH_USR_SerialNum_String(void *SerialNumString)
{   
} 


void USBH_USR_EnumerationDone(void)
{

  
} 


void EXTI0_IRQHandler(void)
{
  static int i = 0;
  
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    if(i ^=1)
    {
      my_fopen();
    }
    else
    {
     // my_fclose();
    }

      EXTI_ClearITPendingBit(EXTI_Line0);
  }
}


USBH_USR_Status USBH_USR_UserInput(void)
{
  USBH_USR_Status usbh_usr_status;
  
  usbh_usr_status = USBH_USR_NO_RESP;  
  
 // if(STM_EVAL_PBGetState(BUTTON_USER) != RESET) 
  {
    
   // usbh_usr_status = USBH_USR_RESP_OK;
    
  } 
  return usbh_usr_status;
}  


void USBH_USR_OverCurrentDetected (void)
{
}


FIL file;
FATFS fatfs;


static uint8_t Explore_Disk (char* path , uint8_t recu_level)
{

  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  char tmp[14];
  
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    while(HCD_IsDeviceConnected(&USB_OTG_Core)) 
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) 
      {
        break;
      }
      if (fno.fname[0] == '.')
      {
        continue;
      }

      fn = fno.fname;
      strcpy(tmp, fn); 

      line_idx++;
      if(line_idx > 9)
      {
        line_idx = 0;
    //    LCD_SetTextColor(Green);
    //    LCD_DisplayStringLine (LCD_LINE_15, "Press Key to continue...       ");
     //   LCD_DisplayStringLine (LCD_LINE_16, "                               ");
      //  LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
        
        /*Key B3 in polling*/
        /*
        while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
                  (STM_EVAL_PBGetState (BUTTON_USER) != SET))
        {
          Toggle_Leds();
          
        }
        */
        
      } 
      
      if(recu_level == 1)
      {
    //    LCD_DbgLog("   |__");
      }
      else if(recu_level == 2)
      {
    //    LCD_DbgLog("   |   |__");
      }
      if((fno.fattrib & AM_MASK) == AM_DIR)
      {
    //    strcat(tmp, "\n"); 
   //     LCD_UsrLog((void *)tmp);
      }
      else
      {
     //   strcat(tmp, "\n"); 
       // LCD_DbgLog((void *)tmp);
      }

      if(((fno.fattrib & AM_MASK) == AM_DIR)&&(recu_level == 1))
      {
        Explore_Disk(fn, 2);
      }
    }
  }
  return res;
}
  
void my_fopen(void)
{


  if(f_mount(0,&fatfs) == FR_OK) 
  {
    Explore_Disk("0:/", 1);
    f_open(&file, "0:STM32.TXT",FA_CREATE_ALWAYS | FA_WRITE);
  }
}

void my_flose(void)
{
  f_close(&file);
  f_mount(0, NULL); 
}

void my__fwrite(void)
{
  static int i=0;
  static char buffer[50];
  uint16_t bytesWritten;
  if(f_o==1) 
  {
    sprintf((char*)buffer, "preved medved %d", i);
    f_write (&file, buffer, sizeof(buffer), (void *)&bytesWritten);
    i++;
    
    f_write (&file, buffer, sizeof(buffer), (void *)&bytesWritten);
    i++;
  }
}



int USBH_USR_MSC_Application(void)
{
  //FIL file;
 // FATFS fatfs;
  //uint16_t bytesWritten;
  //char buffer[] = "preved medved";
  
  printf("");
  
  //f_mount(0, &fatfs);
    
    //if(f_open(&file, "0:STM32.TXT",FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    //{ 

    
      //f_write (&file, buffer, sizeof(buffer), (void *)&bytesWritten);   

    // f_Close();
     // f_mount(0, NULL); 
   // }
    
    return 0;
}    
  


void USBH_USR_DeInit(void)
{
  USBH_USR_ApplicationState = USH_USR_FS_INIT;
}

