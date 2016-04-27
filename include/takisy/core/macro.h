#ifndef macro_h_20160406
#define macro_h_20160406

/*
   The operating system, must be one of: (__os_x__)

    x:
     darwin   - Any Darwin system
     mac      - OS X and iOS
     osx      - OS X
     ios      - iOS
     msdos    - MS-DOS and Windows
     os2      - OS/2
     os2emx   - XFree86 on OS/2 (not PM)
     win32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
     win64    - Win64
     wince    - WinCE (Windows CE 5.0)
     winrt    - WinRT (Windows 8 Runtime)
     win      - Windows (Cygwin, Win32, Win64, WinCE, WinRT)
     cygwin   - Cygwin
     solaris  - Sun Solaris
     hpux     - HP-UX
     ultrix   - DEC Ultrix
     linux    - Linux [has variants]
     freebsd  - FreeBSD [has variants]
     netbsd   - NetBSD
     openbsd  - OpenBSD
     bsdi     - BSD/OS
     irix     - SGI Irix
     osf      - HP Tru64 UNIX
     sco      - SCO OpenServer 5
     unixware - UnixWare 7, Open UNIX 8
     aix      - AIX
     hurd     - GNU Hurd
     dgux     - DG/UX
     reliant  - Reliant UNIX
     dynix    - DYNIX/ptx
     qnx      - QNX [has variants]
     qnx6     - QNX RTP 6.1
     lynx     - LynxOS
     bsd4     - Any BSD 4.4 system
     unix     - Any UNIX BSD/SYSV system
     android  - Android platform

   The following operating systems have variants:
     linux    - both __os_linux__ and __os_android__ are defined when building for Android
              - only __os_linux__ is defined if building for other Linux systems
     qnx      - both __os_qnx__ and __os_blackberry__ are defined when building for Blackberry 10
              - only __os_qnx__ is defined if building for other QNX targets
     freebsd  - __os_freebsd__ is defined only when building for FreeBSD with a BSD userland
              - __os_freebsd_kernel__ is always defined on FreeBSD, even if the userland is from GNU
*/

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) \
    || defined(__xlc__))
#  define __os_darwin__
#  define __os_bsd4__
#  ifdef __LP64__
#    define __os_darwin64__
#  else
#    define __os_darwin32__
#  endif
#elif defined(ANDROID)
#  define __os_android__
#  define __os_linux__
#elif defined(__CYGWIN__)
#  define __os_cygwin__
#elif !defined(SAG_COM) \
    && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) \
    && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define __os_win32__
#  define __os_win64__
#elif !defined(SAG_COM) \
    && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) \
        || defined(__NT__))
#  if defined(WINCE) || defined(_WIN32_WCE)
#    define __os_wince__
#  elif defined(WINAPI_FAMILY)
#    if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
#      define __os_winphone__
#      define __os_winrt__
#    elif WINAPI_FAMILY==WINAPI_FAMILY_APP
#      define __os_winrt__
#    else
#      define __os_win32__
#    endif
#  else
#    define __os_win32__
#  endif
#elif defined(__sun) || defined(sun)
#  define __os_solaris__
#elif defined(hpux) || defined(__hpux)
#  define __os_hpux__
#elif defined(__ultrix) || defined(ultrix)
#  define __os_ultrix__
#elif defined(sinix)
#  define __os_reliant__
#elif defined(__native_client__)
#  define __os_nacl__
#elif defined(__linux__) || defined(__linux)
#  define __os_linux__
#elif defined(__FreeBSD__) || defined(__DragonFly__) \
    || defined(__FreeBSD_kernel__)
#  ifndef __FreeBSD_kernel__
#    define __os_freebsd__
#  endif
#  define __os_freebsd_kernel__
#  define __os_bsd4__
#elif defined(__NetBSD__)
#  define __os_netbsd__
#  define __os_bsd4__
#elif defined(__OpenBSD__)
#  define __os_openbsd__
#  define __os_bsd4__
#elif defined(__bsdi__)
#  define __os_bsdi__
#  define __os_bsd4__
#elif defined(__sgi)
#  define __os_irix__
#elif defined(__osf__)
#  define __os_osf__
#elif defined(_AIX)
#  define __os_aix__
#elif defined(__Lynx__)
#  define __os_lynx__
#elif defined(__GNU__)
#  define __os_hurd__
#elif defined(__DGUX__)
#  define __os_dgux__
#elif defined(__QNXNTO__)
#  define __os_qnx__
#elif defined(_SEQUENT_)
#  define __os_dynix__
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define __os_sco__
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define __os_unixware__
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define __os_unixware__
#elif defined(__INTEGRITY)
#  define __os_integrity__
#elif defined(VXWORKS)
/* there is no "real" VxWorks define - this has to be set in the mkspec! */
#  define __os_vxworks__
#elif defined(__MAKEDEPEND__)
#else
#  error "Takisy has not been ported to this OS!"
#endif

#if defined(__os_cygwin__) || defined(__os_win32__) || defined(__os_win64__) \
    || defined(__os_wince__) || defined(__os_winrt__)
#  define __os_win__
#endif

#if defined(__os_darwin__)
#  define __os_mac__
#  if defined(__os_darwin64__)
#     define __os_mac64__
#  elif defined(__os_darwin32__)
#     define __os_mac32__
#  endif
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#     define __os_ios__
#  elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
#     define __os_osx__
#     define __os_macx__ // compatibility synonym
#  endif
#endif

#if defined(__os_win__) && !defined(__os_cygwin__)
#  undef __os_unix__
#elif !defined(__os_unix__)
#  define __os_unix__
#endif

#endif // macro_h_20160406
