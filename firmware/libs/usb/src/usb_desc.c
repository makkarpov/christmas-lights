#include "usb_lib.h"
#include "usb_desc.h"

#define WORD(w) ((w) & 0xFF), (((w) >> 8) & 0xFF)

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] = {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    WORD(0x0200),               /*bcdUSB */
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x20,                       /*bMaxPacketSize*/
	  WORD(0x1209),							  /*idVendor*/
	  WORD(0x1488),               /*idProduct*/
	  WORD(0x0200),               /*bcdDevice rel. 2.00*/
    1,                          /*Manufacturer string*/
    2,                          /*Product string*/
    3,                          /*Serial number string*/
    0x01                        /*bNumConfigurations*/
};

/* USB Configuration Descriptor */
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] = {
    0x09,                                /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,   /* bDescriptorType: Configuration */
    WORD(CUSTOMHID_SIZ_CONFIG_DESC),     /* wTotalLength: Bytes returned */
    0x01,                                /* bNumInterfaces: 1 interface */
    0x01,                                /* bConfigurationValue: Configuration value */
    0x00,                                /* iConfiguration: Index of string descriptor describing the configuration*/
    0xC0,                                /* bmAttributes: Self powered */
    0x32,                                /* MaxPower 100 mA: this current is used for detecting Vbus */

    0x09,                                /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,       /* bDescriptorType: Interface descriptor type */
    0x00,                                /* bInterfaceNumber: Number of Interface */
    0x00,                                /* bAlternateSetting: Alternate setting */
    0x03,                                /* bNumEndpoints */
    0xFF,                                /* bInterfaceClass: Vendor */
    0xFF,                                /* bInterfaceSubClass */
    0xFF,                                /* nInterfaceProtocol */
    0,                                   /* iInterface: Index of string descriptor */
		
    0x07,                                /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,        /* bDescriptorType: */
    0x81,                                /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                                /* bmAttributes: Interrupt endpoint */
    WORD(16),                            /* wMaxPacketSize: 16 Bytes max */
    0x20,                                /* bInterval: Polling Interval (32 ms) */
    
    0x07,	                               /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	       /* bDescriptorType */
    0x01,	                               /* bEndpointAddress:	Endpoint Address (OUT) */
    0x03,	                               /* bmAttributes: Interrupt endpoint */
    WORD(16), 	                         /* wMaxPacketSize: 16 Bytes max  */
    0x20,	                               /* bInterval: Polling Interval (20 ms) */
    
		0x07,	                               /* bLength: Endpoint Descriptor size */
		USB_ENDPOINT_DESCRIPTOR_TYPE,	       /* bDescriptorType */
		0x02,	                               /* bEndpointAddress:	Endpoint Address (OUT) */
		0x01,	                               /* bmAttributes: Isochronous endpoint */
		WORD(ISOC_EP_PACKET_SIZE),           /* wMaxPacketSize */
		0x01				                         /* bInterval: 1 for isochonous */
};
	
/* USB language ID descriptors */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] = {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
}; /* LangID = 0x0409: U.S. English */

/* Vendor: "Maxim Karpov /makkarpov.ru/" */
const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] = {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    'M', 0, 'a', 0, 'x', 0, 'i', 0, 'm', 0, ' ', 0, 'K', 0, 'a', 0,
    'r', 0, 'p', 0, 'o', 0, 'v', 0, ' ', 0, '/', 0, 'm', 0, 'a', 0,
    'k', 0, 'k', 0, 'a', 0, 'r', 0, 'p', 0, 'o', 0, 'v', 0, '.', 0,
		'r', 0, 'u', 0, '/', 0
};

/* Product: "Christmas lights" */
const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] = {
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'C', 0, 'h', 0, 'r', 0, 'i', 0, 's', 0, 't', 0, 'm', 0,
    'a', 0, 's', 0, ' ', 0, 'l', 0, 'i', 0, 'g', 0, 'h', 0,
    't', 0, 's', 0
};

/* Serial: "CL1:XXXXXXXX" */
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] = {
    CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'C', 0, 'L', 0, '1', 0, ':', 0
};
