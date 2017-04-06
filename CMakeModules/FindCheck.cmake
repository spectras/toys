# - Try to find the CHECK libraries
#  Once done this will define
#
#  CHECK_FOUND - system has check
#  CHECK_INCLUDE_DIRS - the check include directory
#  CHECK_LIBRARIES - check library
#
#  Copyright (c) 2007 Daniel Gollub <gollub@b1-systems.de>
#  Copyright (c) 2007-2009 Bjoern Ricks  <bjoern.ricks@gmail.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Look for CHECK include dir and libraries
IF( NOT CHECK_FOUND AND NOT PKG_CONFIG_FOUND )

	FIND_PATH( CHECK_INCLUDE_DIRS check.h )
	FIND_LIBRARY( CHECK_LIBRARIES NAMES check )

	IF ( CHECK_INCLUDE_DIRS AND CHECK_LIBRARIES )
		SET( CHECK_FOUND 1 )
		IF ( NOT Check_FIND_QUIETLY )
			MESSAGE ( STATUS "Found Check: ${CHECK_LIBRARIES}" )
		ENDIF ( NOT Check_FIND_QUIETLY )
		SET( CHECK_LIBRARIES ${CHECK_LIBRARIES} -lsubunit -lm -lrt -lpthread )

		ELSE ( CHECK_INCLUDE_DIRS AND CHECK_LIBRARIES )
		IF ( Check_FIND_REQUIRED )
			MESSAGE( FATAL_ERROR "Could NOT find Check" )
		ELSE ( Check_FIND_REQUIRED )
			IF ( NOT Check_FIND_QUIETLY )
				MESSAGE( STATUS "Could NOT find Check" )
			ENDIF ( NOT Check_FIND_QUIETLY )
		ENDIF ( Check_FIND_REQUIRED )
	ENDIF ( CHECK_INCLUDE_DIRS AND CHECK_LIBRARIES )
ENDIF( NOT CHECK_FOUND AND NOT PKG_CONFIG_FOUND )

# Hide advanced variables from CMake GUIs
MARK_AS_ADVANCED( CHECK_INCLUDE_DIRS CHECK_LIBRARIES )
