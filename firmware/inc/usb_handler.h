#ifndef _USB_HANDLER_H_
#define _USB_HANDLER_H_

#ifdef __cplusplus
extern "C" {
#endif

void USB_Initialize(void);
void USB_PopulateSerialNumber(void);
void USB_PullupConfig(char enable);
void EP1_IN_Callback(void);
void EP1_OUT_Callback(void);
void EP2_OUT_Callback(void);
	
#ifdef __cplusplus
};
#endif

#endif // _USB_HANDLER_H_
