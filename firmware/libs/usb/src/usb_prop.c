/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processings related to Custom HID Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#include "usb_handler.h"

/* Private typedef -----------------------------------------------------------*/ 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;
__IO uint8_t EXTI_Enable;
__IO uint8_t Request = 0;
uint8_t Report_Buf[2];   
/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    CustomHID_init,
    CustomHID_Reset,
    CustomHID_Status_In,
    CustomHID_Status_Out,
    CustomHID_Data_Setup,
    CustomHID_NoData_Setup,
    CustomHID_Get_Interface_Setting,
    CustomHID_GetDeviceDescriptor,
    CustomHID_GetConfigDescriptor,
    CustomHID_GetStringDescriptor,
    0,
    0x20 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    CustomHID_GetConfiguration,
    CustomHID_SetConfiguration,
    CustomHID_GetInterface,
    CustomHID_SetInterface,
    CustomHID_GetStatus,
    CustomHID_ClearFeature,
    CustomHID_SetEndPointFeature,
    CustomHID_SetDeviceFeature,
    CustomHID_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)CustomHID_DeviceDescriptor,
    CUSTOMHID_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)CustomHID_ConfigDescriptor,
    CUSTOMHID_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(uint8_t*)CustomHID_StringLangID, CUSTOMHID_SIZ_STRING_LANGID},
    {(uint8_t*)CustomHID_StringVendor, CUSTOMHID_SIZ_STRING_VENDOR},
    {(uint8_t*)CustomHID_StringProduct, CUSTOMHID_SIZ_STRING_PRODUCT},
    {(uint8_t*)CustomHID_StringSerial, CUSTOMHID_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*CustomHID_SetReport_Feature function prototypes*/
uint8_t *CustomHID_SetReport_Feature(uint16_t Length);

/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CustomHID_init.
* Description    : Custom HID init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_init(void)
{
  /* Update the serial number string descriptor with the data from the unique 
  ID*/
  USB_PopulateSerialNumber();
    
  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : CustomHID_Reset.
* Description    : Custom HID reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Reset(void)
{
  /* Set CustomHID_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/
  
  /* Current Feature initialization */
  pInformation->Current_Feature = CustomHID_ConfigDescriptor[7];
 
  SetBTABLE(BTABLE_ADDRESS);

	// Initialize control endpoint (0)
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	Clear_Status_Out(ENDP0);
	SetEPTxStatus(ENDP0, EP_TX_STALL);
	SetEPRxStatus(ENDP0, EP_RX_VALID);
	
	SetEPType(ENDP1, EP_INTERRUPT);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPRxAddr(ENDP1, ENDP1_RXADDR);
	SetEPTxCount(ENDP1, 16);
	SetEPRxCount(ENDP1, 16);
	SetEPTxStatus(ENDP1, EP_TX_NAK);
	SetEPRxStatus(ENDP1, EP_RX_VALID);
	
	SetEPType(ENDP2, EP_ISOCHRONOUS);
	SetEPDblBuffAddr(ENDP2, ENDP2_RXBUF1, ENDP2_RXBUF2);
	SetEPDblBuffCount(ENDP2, EP_DBUF_OUT, ISOC_EP_PACKET_SIZE);
	ClearDTOG_RX(ENDP2);
	ClearDTOG_TX(ENDP2);
	ToggleDTOG_TX(ENDP2);
	SetEPTxStatus(ENDP2, EP_TX_DIS);
	SetEPRxStatus(ENDP2, EP_RX_VALID);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  bDeviceState = ATTACHED;
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Update the device state to configured and command the ADC 
*                  conversion.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_SetConfiguration(void) {
  if (pInformation->Current_Configuration != 0) {
    /* Device configured */
    bDeviceState = CONFIGURED;
  } else {
		bDeviceState = ADDRESSED;
	}
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : CustomHID_Status_In.
* Description    : Joystick status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_In(void) {
	// Do something with Report_Buf
}

/*******************************************************************************
* Function Name  : CustomHID_Status_Out
* Description    : Joystick status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_Out (void) {
	// Xfer completed
}

/*******************************************************************************
* Function Name  : CustomHID_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_Data_Setup(uint8_t RequestNo)
{
	return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : CustomHID_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL))
  {
    return CustomHID_SetProtocol();
  }

  else
  {
    return USB_UNSUPPORT;
  }
}

/*******************************************************************************
* Function Name  : CustomHID_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *CustomHID_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else 
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : CustomHID_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT CustomHID_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0) {
    return USB_UNSUPPORT;
  } else if (Interface > 0) {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT CustomHID_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protocol value.
*******************************************************************************/
uint8_t *CustomHID_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
