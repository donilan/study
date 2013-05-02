//获取硬盘ID信息及网卡MAC地址的代码
#include "stdafx.h"
#include <atlbase.h>
#include <atlconv.h>
#include "iphlpapi.h"
#pragma comment ( lib, "Iphlpapi.lib" )

BOOLEAN getActiveMac( char* sMac )
{ 
	BOOLEAN bRtn = FALSE;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) 
	{
		free (pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen);
	}
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;
		char szMac[20] = {0};
		char szAddress[4] = {0};
		while (pAdapter) 
		{ 

			if ( strcmp( pAdapter->IpAddressList.IpAddress.String ,"0.0.0.0" ) != 0 )
			{
				for (UINT i = 0; i < pAdapter->AddressLength; i++)
				{ 
					sprintf_s( szAddress , "%02X", pAdapter->Address[i] );
					strcat_s( szMac , szAddress );
					if( i != pAdapter->AddressLength - 1 )
					{
						strcat_s( szMac , "-" );
					}
				}
				break;
			}
			pAdapter = pAdapter->Next;
		}
		memcpy( sMac , szMac , strlen( szMac ) );
	}
	else 
	{
		printf("Call to GetAdaptersInfo failed.\n");
	}
	return bRtn;
}
