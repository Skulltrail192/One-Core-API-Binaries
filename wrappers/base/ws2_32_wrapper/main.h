#define WIN32_NO_STATUS

#include <stdarg.h>
#include <math.h>

#define NONAMELESSUNION
#define NONAMELESSSTRUCT
#include <windef.h>
#include <winbase.h>

#include <wine/debug.h>
#include <wine/list.h>
#include <wine/unicode.h>
#include <winsock.h>
#include <winsock2.h>

#define POLLHUP                    0x0002
#define WS_POLLHUP                 0x0002
#define WS_POLLNVAL                0x0004

#define POLLERR                    0x0001
#define POLLHUP                    0x0002
#define POLLNVAL                   0x0004
#define POLLWRNORM                 0x0010
#define POLLWRBAND                 0x0020
#define POLLRDNORM                 0x0100
#define POLLRDBAND                 0x0200
#define POLLPRI                    0x0400
#define POLLIN                     (POLLRDNORM|POLLRDBAND)
#define POLLOUT                    (POLLWRNORM)

#define WS_POLLERR                 0x0001
#define WS_POLLHUP                 0x0002
#define WS_POLLNVAL                0x0004
#define WS_POLLWRNORM              0x0010
#define WS_POLLWRBAND              0x0020
#define WS_POLLRDNORM              0x0100
#define WS_POLLRDBAND              0x0200
#define WS_POLLPRI                 0x0400
#define WS_POLLIN                  (WS_POLLRDNORM|WS_POLLRDBAND)
#define WS_POLLOUT                 (WS_POLLWRNORM)
#define WSA_NOT_ENOUGH_MEMORY      (ERROR_NOT_ENOUGH_MEMORY)
# define WS_AI_DISABLE_IDN_ENCODING   0x00080000

#define WS_INET6_ADDRSTRLEN     65
#define NS_DNS         12

#define MAP_OPTION(opt) { WS_##opt, opt }

#define WSAEVENT HANDLE

/* getaddrinfo error codes */
#define EAI_AGAIN WSATRY_AGAIN
#define EAI_BADFLAGS WSAEINVAL
#define EAI_FAIL WSANO_RECOVERY
#define EAI_FAMILY WSAEAFNOSUPPORT
#define EAI_MEMORY WSA_NOT_ENOUGH_MEMORY
#define EAI_NODATA EAI_NONAME
#define EAI_NOSECURENAME WSA_SECURE_HOST_NOT_FOUND
#define EAI_NONAME WSAHOST_NOT_FOUND
#define EAI_SERVICE WSATYPE_NOT_FOUND
#define EAI_SOCKTYPE WSAESOCKTNOSUPPORT
#define EAI_IPSECPOLICY WSA_IPSEC_NAME_POLICY_ERROR

#define WSA_INVALID_HANDLE         (ERROR_INVALID_HANDLE)

static const unsigned __int64 epoch = ((unsigned __int64) 116444736000000000ULL);

struct pollfd
{
     int fd;
     short events;
     short revents;
};

typedef struct /*WS(pollfd)*/
{
    SOCKET fd;
    SHORT events;
    SHORT revents;
} WSAPOLLFD;

typedef enum _WSC_PROVIDER_INFO_TYPE
{
    ProviderInfoLspCategories,
    ProviderInfoAudit,
} WSC_PROVIDER_INFO_TYPE;

int poll( struct pollfd *fds, unsigned int count, int timeout );

typedef struct addrinfoexW {
  int                ai_flags;
  int                ai_family;
  int                ai_socktype;
  int                ai_protocol;
  size_t             ai_addrlen;
  PWSTR              ai_canonname;
  struct sockaddr    *ai_addr;
  void               *ai_blob;
  size_t             ai_bloblen;
  LPGUID             ai_provider;
  struct addrinfoexW *ai_next;
} ADDRINFOEXW, *PADDRINFOEXW, *LPADDRINFOEXW;

typedef struct _WSAOVERLAPPED {
  DWORD    Internal;
  DWORD    InternalHigh;
  DWORD    Offset;
  DWORD    OffsetHigh;
  WSAEVENT hEvent;
} WSAOVERLAPPED, *LPWSAOVERLAPPED;

struct timezone {
    int tz_minuteswest; /* minutes W of Greenwich */
    int tz_dsttime;     /* type of dst correction */
};

typedef
void
(CALLBACK * LPLOOKUPSERVICE_COMPLETION_ROUTINE)(
    _In_      DWORD    dwError,
    _In_      DWORD    dwBytes,
    _In_      LPWSAOVERLAPPED lpOverlapped
    );
	
struct getaddrinfo_args
{
    OVERLAPPED *overlapped;
    LPLOOKUPSERVICE_COMPLETION_ROUTINE completion_routine;
    ADDRINFOEXW **result;
    char *nodename;
    char *servname;
};	

typedef struct addrinfo {
  int             ai_flags;
  int             ai_family;
  int             ai_socktype;
  int             ai_protocol;
  size_t          ai_addrlen;
  char            *ai_canonname;
  struct sockaddr *ai_addr;
  struct addrinfo *ai_next;
} ADDRINFOA, *PADDRINFOA;

typedef struct addrinfoW {
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  size_t ai_addrlen;
  PWSTR ai_canonname;
  struct sockaddr *ai_addr;
  struct addrinfoW *ai_next;
} ADDRINFOW, *PADDRINFOW;

typedef struct addrinfoexA {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    SIZE_T ai_addrlen;
    char *ai_canonname;
    struct sockaddr *ai_addr;
    void *ai_blob;
    SIZE_T ai_bloblen;
    GUID *ai_provider;
    struct addrinfoexA *ai_next;
} ADDRINFOEXA, *PADDRINFOEXA, *LPADDRINFOEXA;

void WINAPI FreeAddrInfoExW(ADDRINFOEXW*);

INT
GetAddrInfoW(
  _In_opt_ PCWSTR pNodeName,
  _In_opt_ PCWSTR pServiceName,
  _In_opt_ const ADDRINFOW *pHints,
  _Outptr_ PADDRINFOW *ppResult);
  
INT
WINAPI
getaddrinfo(
  _In_opt_ PCSTR pNodeName,
  _In_opt_ PCSTR pServiceName,
  _In_opt_ const ADDRINFOA *pHints,
  _Outptr_ PADDRINFOA *ppResult);  
  
VOID
WINAPI
freeaddrinfo(
  _In_opt_ PADDRINFOA pAddrInfo); 
  
WINAPI IdnToAscii(DWORD,LPCWSTR,INT,LPWSTR,INT);
WINAPI TrySubmitThreadpoolCallback(PTP_SIMPLE_CALLBACK,void*,TP_CALLBACK_ENVIRON*);