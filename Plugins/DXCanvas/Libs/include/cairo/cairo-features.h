/* cairo features define for msvc */
#ifndef CAIRO_FEATURES_H
#define CAIRO_FEATURES_H

#ifdef  __cplusplus
# define CAIRO_BEGIN_DECLS  extern "C" {
# define CAIRO_END_DECLS    }
#else
# define CAIRO_BEGIN_DECLS
# define CAIRO_END_DECLS
#endif

//#define CAIRO_NO_MUTEX 1

static void
free_data(void* data)
{
	free(data);
}

#ifndef cairo_public
	#ifdef LIBCAIRO_EXPORTS
		#define cairo_public __declspec(dllexport)
	#else
		#define cairo_public __declspec(dllimport)
	#endif
#endif

#define CAIRO_WIN32_STATIC_BUILD 1

#define HAVE_WINDOWS_H 1

#define CAIRO_HAS_FC_FONT 1
#define CAIRO_HAS_FT_FONT 1
#define CAIRO_HAS_GOBJECT_FUNCTIONS 1
#define CAIRO_HAS_IMAGE_SURFACE 1
#define CAIRO_HAS_PDF_SURFACE 1
#define CAIRO_HAS_PNG_FUNCTIONS 1
#define CAIRO_HAS_PS_SURFACE 1
#define CAIRO_HAS_RECORDING_SURFACE 1
#define CAIRO_HAS_SVG_SURFACE 1
#define CAIRO_HAS_USER_FONT 1
#define CAIRO_HAS_WIN32_FONT 1
#define CAIRO_HAS_WIN32_SURFACE 1

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "cairo"
#define PACKAGE_TARNAME "cairo"
#define PACKAGE_VERSION "1.10.2"
#define PACKAGE_STRING "cairo 1.10.2"
#define PACKAGE_BUGREPORT "http://bugs.freedesktop.org/enter_bug.cgi?product=cairo"
#endif

#endif
