
#ifndef LIBCITYGML_EXPORT_H
#define LIBCITYGML_EXPORT_H

#ifdef CITYGML_STATIC_DEFINE
#  define LIBCITYGML_EXPORT
#  define CITYGML_NO_EXPORT
#else
#  ifndef LIBCITYGML_EXPORT
#    ifdef citygml_EXPORTS
        /* We are building this library */
#      define LIBCITYGML_EXPORT 
#    else
        /* We are using this library */
#      define LIBCITYGML_EXPORT 
#    endif
#  endif

#  ifndef CITYGML_NO_EXPORT
#    define CITYGML_NO_EXPORT 
#  endif
#endif

#ifndef CITYGML_DEPRECATED
#  define CITYGML_DEPRECATED __declspec(deprecated)
#endif

#ifndef CITYGML_DEPRECATED_EXPORT
#  define CITYGML_DEPRECATED_EXPORT LIBCITYGML_EXPORT CITYGML_DEPRECATED
#endif

#ifndef CITYGML_DEPRECATED_NO_EXPORT
#  define CITYGML_DEPRECATED_NO_EXPORT CITYGML_NO_EXPORT CITYGML_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CITYGML_NO_DEPRECATED
#    define CITYGML_NO_DEPRECATED
#  endif
#endif

#endif /* LIBCITYGML_EXPORT_H */
