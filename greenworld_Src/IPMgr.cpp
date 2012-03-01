/// \file IPMgr.cpp 
/// Implementation of the IPManager class.
/// Copyright Ian Parberry, 2004.
/// Last updated August 11, 2004.

// REMINDER: link to ws2_32.lib

#include "debug.h"
#include "IPMgr.h" 

#ifdef DEBUG_ON

/// Winsock error descriptions.

WSAEDESCRIPTION g_WSAErrorDescriptions[]={
  WSAEINTR,
  "WSAEINTR: Interrupted function call",
  WSAEBADF,
  "WSAEBADF: Bad file number",
  WSAEACCES,
  "WSAEACCES: Permission denied",
  WSAEFAULT,
  "WSAEFAULT: Address fault",
  WSAEINVAL,
  "WSAEINVAL: Invalid argument",
  WSAEMFILE,
  "WSAEMFILE: Too many open files",
  WSAEWOULDBLOCK,
  "WSAEWOULDBLOCK: Resource temporarily unavailable (Would block)",
  WSAEINPROGRESS,
  "WSAEINPROGRESS: Operation now in progress",
  WSAEALREADY,
  "WSAEALREADY: Operation already in progress",
  WSAENOTSOCK,
  "WSAENOTSOCK: Socket operation on non-socket",
  WSAEDESTADDRREQ,
  "WSAEDESTADDRREQ: Destination address required",
  WSAEMSGSIZE,
  "WSAEMSGSIZE: Message too long",
  WSAEPROTOTYPE,
  "WSAEPROTOTYPE: Protocol wrong type for socket",
  WSAENOPROTOOPT,
  "WSAENOPROTOOPT: Bad protocol option",
  WSAEPROTONOSUPPORT,
  "WSAEPROTONOSUPPORT: Protocol not supported",
  WSAESOCKTNOSUPPORT,
  "WSAESOCKTNOSUPPORT: Socket type not supported",
  WSAEOPNOTSUPP,
  "WSAEOPNOTSUPP: Operation not supported",
  WSAEPFNOSUPPORT,
  "WSAEPFNOSUPPORT: Protocol family not supported",
  WSAEAFNOSUPPORT,
  "WSAEAFNOSUPPORT: Address family not supported by protocol family",
  WSAEADDRINUSE,
  "WSAEADDRINUSE: Address already in use",
  WSAEADDRNOTAVAIL,
  "WSAEADDRNOTAVAIL: Cannot assign requested address",
  WSAENETDOWN,
  "WSAENETDOWN: Network is down",
  WSAENETUNREACH,
  "WSAENETUNREACH: Network is unreachable",
  WSAENETRESET,
  "WSAENETRESET: Network dropped connection on reset",
  WSAECONNABORTED,
  "WSAECONNABORTED: Software caused connection abort",
  WSAECONNRESET,
  "WSAECONNRESET: Connection reset by peer",
  WSAENOBUFS,
  "WSAENOBUFS: No buffer space available",
  WSAEISCONN,
  "WSAEISCONN: Socket is already connected",
  WSAENOTCONN,
  "WSAENOTCONN: Socket is not connected",
  WSAESHUTDOWN,
  "WSAESHUTDOWN: Cannot send after socket shutdown",
  WSAETOOMANYREFS,
  "WSAETOOMANYREFS: Too many references",
  WSAETIMEDOUT,
  "WSAETIMEDOUT: Connection timed out",
  WSAECONNREFUSED,
  "WSAECONNREFUSED: Connection refused",
  WSAELOOP,
  "WSAELOOP: Loop",
  WSAENAMETOOLONG,
  "WSAENAMETOOLONG: Name too long",
  WSAEHOSTDOWN,
  "WSAEHOSTDOWN: Host is down",
  WSAEHOSTUNREACH,
  "WSAEHOSTUNREACH: No route to host",
  WSAENOTEMPTY,
  "WSAENOTEMPTY: Directory not empty",
  WSAEPROCLIM,
  "WSAEPROCLIM: Too many processes",
  WSAEUSERS,
  "WSAEUSERS: Too many users",
  WSAEDQUOT,
  "WSAEDQUOT: Disk quota",
  WSAESTALE,
  "WSAESTALE: Stale",
  WSAEREMOTE,
  "WSAEREMOTE: Remote",
  WSASYSNOTREADY,
  "WSASYSNOTREADY: Network subsystem is unavailable",
  WSAVERNOTSUPPORTED,
  "WSAVERNOTSUPPORTED: WINSOCK.DLL version out of range",
  WSANOTINITIALISED,
  "WSANOTINITIALISED: Successful WSAStartup() not yet performed",
  WSAEDISCON,
  "WSAEDISCON: Graceful shutdown in progress",
  WSAHOST_NOT_FOUND,
  "WSAHOST_NOT_FOUND: Host not found",
  WSATRY_AGAIN,
  "WSATRY_AGAIN: Non-authoritative host not found",
  WSANO_RECOVERY,
  "WSANO_RECOVERY: Non-recoverable error",
  WSANO_DATA,
  "WSANO_DATA: Valid name, no data record of requested type",
#ifdef _WINSOCK2API_
  WSAENOMORE,
  "WSAENOMORE: ",
  WSAECANCELLED,
  "WSAECANCELLED: Cancelled",
  WSAEINVALIDPROCTABLE,
  "WSAEINVALIDPROCTABLE: Invalid procedure table from service provider",
  WSAEINVALIDPROVIDER,
  "WSAEINVALIDPROVIDER: Invalid service provider version number",
  WSAEPROVIDERFAILEDINIT,
  "WSAEPROVIDERFAILEDINIT: Unable to initialize a service provider",
  WSASYSCALLFAILURE,
  "WSASYSCALLFAILURE: System call failure",
  WSASERVICE_NOT_FOUND,
  "WSASERVICE_NOT_FOUND: Service not found",
  WSATYPE_NOT_FOUND,
  "WSATYPE_NOT_FOUND: Type not found",
  WSA_E_NO_MORE,
  "WSA_E_NO_MORE: ",
  WSA_E_CANCELLED,
  "WSA_E_CANCELLED: ",
  WSAEREFUSED,
  "WSAEREFUSED: Refused",
#endif
};

//IP manager class

/// Opens up a UDP datagram socket to the given IP address and socket.
/// \param address IP address
/// \param port port number

IPManager::IPManager(char* address,int port){ //constructor
  
  WSADATA wsaData; //winsock data
  m_bInitialized=TRUE; //start assuming all is OK

  //start up winsock
  m_bInitialized=m_bInitialized&&WSAStartup(WINSOCK_VERSION,&wsaData)==0; //did we init OK?
  m_bInitialized=m_bInitialized&&wsaData.wVersion==WINSOCK_VERSION; //did we get the right winsock version?

  //create a UDP/IP datagram socket
  m_Socket=INVALID_SOCKET;
  if(m_bInitialized)m_Socket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  m_bInitialized=m_bInitialized&&m_Socket!=INVALID_SOCKET;

  //do housekeeping and exit
  if(m_bInitialized){
    //fill in the address structure for the client
    m_saClient.sin_family=AF_INET;
    m_saClient.sin_port=htons(port);  //port number
    unsigned int addr=inet_addr((LPCTSTR)address); //ip address
    CopyMemory((char*)&(m_saClient.sin_addr),(char*)&addr,4); //client's address
  }
  else WSACleanup(); //failed - clean up and bail out
}

IPManager::~IPManager(){ //destructor

  if(m_bInitialized){
    closesocket(m_Socket); //close the socket
    WSACleanup(); //clean up
  }

}

/// Format a Winsock error description.

LPCTSTR IPManager::WinsockErrorDescription(int nErrorCode){

  const int size=sizeof(g_WSAErrorDescriptions)/sizeof(g_WSAErrorDescriptions[0]);

  for(int i=0; i<size; i++)
    if(g_WSAErrorDescriptions[i].nErrorCode==nErrorCode)
      return(g_WSAErrorDescriptions[i].szDescription);

  return("Unknown error");
}

/// \param message message string to be sent in the packet
/// \param length length of message string to be sent

BOOL IPManager::SendPacket(char* message,int length){ //send  packet

  if(!m_bInitialized)return FALSE; //bail if not ready    

  int nRet=sendto(m_Socket,message,length,0,(LPSOCKADDR)&m_saClient,
    sizeof(struct sockaddr));     

  if(nRet==SOCKET_ERROR)return FALSE;
  return TRUE;
}

#endif
