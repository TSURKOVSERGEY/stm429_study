#include "usbh_core.h"
#include "usbh_msc_core.h"
#include "usbh_usr.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;
__ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;




void main(void)
{
  
   USBH_Init(&USB_OTG_Core, 
            USB_OTG_HS_CORE_ID,
            &USB_Host,
            &USBH_MSC_cb, 
            &USR_cb);
  
  while(1)
  {
        USBH_Process(&USB_OTG_Core, &USB_Host);
  }
  

}
