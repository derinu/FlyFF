#if !defined(AFX_SHOPPING_CART_CARTITEM_STRUCT111)
#define AFX_SHOPPING_CART_CARTITEM_STRUCT111

#ifdef __SHOPPING_CART


#define MAX_CART_ITEM 12

struct CART_ITEM
{
#ifdef __CLIENT
	CItemElem*	pItemElem[MAX_CART_ITEM];
#endif //__CLIENT
	DWORD		dwObjID[MAX_CART_ITEM];
	int			dwItemID[MAX_CART_ITEM];
	CHAR		cTab[MAX_CART_ITEM];
	DWORD		dwNum[MAX_CART_ITEM];
	int			nCount;
	void		Serialize( CAr &ar );
};
#endif //__SHOPPING_CART

#endif //AFX_SHOPPING_CART_CARTITEM_STRUCT111