#ifndef sysdet_h_20160406
#define sysdet_h_20160406

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

#endif // sysdet_h_20160406
