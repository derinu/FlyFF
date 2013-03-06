#include "stdafx.h"
#include "ShoppingCart.h"

void CART_ITEM::Serialize( CAr & ar )
{
	if( ar.IsStoring() )
	{
#ifdef __CLIENT
		int nCount = 0;
		for( int i = 0; i < MAX_CART_ITEM; i++ )
			if( pItemElem[i] )
				nCount++;

		ar << nCount;
		for( int i = 0; i < MAX_CART_ITEM; i++ )
		{
			if( pItemElem[i] )
			{
				ar << pItemElem[i]->m_dwObjId << dwItemID[i] << cTab[i] << dwNum[i];
			}
		}
#endif //__CLIENT
	}
	else
	{
		ar >> nCount;
		if( nCount > MAX_CART_ITEM )
		{
			Error( "Hacking Attempt wrong BuyCart Count %i", nCount );
			return;
		}
		for( int i = 0; i < nCount; i++ )
		{
			ar >> dwObjID[i] >> dwItemID[i] >> cTab[i] >> dwNum[i];
		}
	}
}