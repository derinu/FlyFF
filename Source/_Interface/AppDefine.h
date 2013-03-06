#ifndef __APPDEFINE_H__
#define __APPDEFINE_H__

#include "ResData.h"           // 윈도 고유 ID 모음 


#ifdef __SHOPPING_CART
const UINT APP_SHOP_   = APP_MARKET;
#else //__SHOPPING_CART
const UINT APP_SHOP_   = APP_SHOPEX;
#endif //__SHOPPING_CART
const APP_CONFIRM_BUY_ = APP_CONFIRM_BUYEX;

#if __VER >= 11 // __CSC_VER11_4
const APP_MESSENGER_   = APP_MESSENGER_EX;
#else //__CSC_VER11_4
const APP_MESSENGER_   = APP_MESSENGER;
#endif //__CSC_VER11_4

#endif