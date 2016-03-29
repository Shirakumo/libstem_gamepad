#ifndef STEM_GAMEPAD_EXPORT_H
#define STEM_GAMEPAD_EXPORT_H

#ifdef _MSC_VER
#  ifdef STEM_GAMEPAD_STATIC_DEFINE
#    define STEM_GAMEPAD_EXPORT
#    define STEM_GAMEPAD_NO_EXPORT
#  else
#    ifndef STEM_GAMEPAD_EXPORT
#      ifdef stem_gamepad_EXPORTS
#        define STEM_GAMEPAD_EXPORT __declspec(dllexport)
#      else
#        define STEM_GAMEPAD_EXPORT __declspec(dllimport)
#      endif
#    endif
#  endif
#else
#  define STEM_GAMEPAD_EXPORT
#  define STEM_GAMEPAD_NO_EXPORT
#endif

#endif
