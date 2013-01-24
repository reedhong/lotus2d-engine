#
# $Copyright: 
# ----------------------------------------------------------------
# This confidential and proprietary software may be used only as
# authorised by a licensing agreement from ARM Limited
#   (C) COPYRIGHT 2008-2011 ARM Limited
#       ALL RIGHTS RESERVED
# The entire notice above must be reproduced on all authorised
# copies and copies may only be made to the extent permitted
# by a licensing agreement from ARM Limited.
# $
#

KHRONOS_HEADERS = ../../../include

INCLUDES = include/
SOURCES  = src/cube.c src/shader.c src/window.c src/matrix.c

LIBPATHS = /LIBPATH:../../../lib
LIBS     = libGLESv2.lib libEGL.lib user32.lib

cube:
	cl /I$(KHRONOS_HEADERS) /I$(INCLUDES) /c $(SOURCES)
	link /OUT:cube.exe cube.obj shader.obj matrix.obj window.obj $(LIBPATHS) $(LIBS)

debug:
	cl /I$(KHRONOS_HEADERS) /I$(INCLUDES) /c $(SOURCES) /Zi /D DEBUG
	link /OUT:cube.exe cube.obj shader.obj matrix.obj window.obj $(LIBPATHS) $(LIBS)
	
clean:
	del cube.exe cube.obj shader.obj matrix.obj window.obj

