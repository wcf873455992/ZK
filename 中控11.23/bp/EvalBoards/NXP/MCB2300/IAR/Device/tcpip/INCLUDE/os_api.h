#ifndef	__os_api_h
#define	__os_api_h

//*------------------------------- �ꡢ���������������ͼ��ṹ�嶨�� ---------------------------------
typedef	OS_EVENT	*PHANDLER;

//* ����һ���µ��ź���
OS_EVENT *OSAPIBlockNew(UBYTE ubPIP);
//* �˳�������Դ
#define		OSAPIBlockExit(hBlock)			OSMutexPost(hBlock)
BYTE OSAPIBlockEnter(PHANDLER hBlock, UWORD uwWaitMS);
#endif
