## ---------------------------------------------------------------------
##
## Copyright (C) 2012 - 2015 by the deal.II authors
##
## This file is part of the deal.II library.
##
## The deal.II library is free software; you can use it, redistribute
## it, and/or modify it under the terms of the GNU Lesser General
## Public License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## The full text of the license can be found in the file LICENSE at
## the top level of the deal.II distribution.
##
## ---------------------------------------------------------------------

#
# Configure config.h and revision.h
#
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/deal2lkit/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/deal2lkit/config.h
  )

CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/deal2lkit/revision.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/deal2lkit/revision.h
  )


#
# Add a rule for how to install the header files:
#
INSTALL(DIRECTORY deal2lkit
  DESTINATION ${D2K_INCLUDE_RELDIR}
  COMPONENT library
  FILES_MATCHING PATTERN "*.h"
  )

INSTALL(DIRECTORY external
  DESTINATION ${D2K_INCLUDE_RELDIR}
  COMPONENT library
  FILES_MATCHING PATTERN "*.h"
  )
    
#
# and don't forget to install all generated header files, too:
#
INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/deal2lkit
  DESTINATION ${D2K_INCLUDE_RELDIR}
  COMPONENT library
  FILES_MATCHING PATTERN "*.h"
  )

INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/external
  DESTINATION ${D2K_INCLUDE_RELDIR}
  COMPONENT library
  FILES_MATCHING PATTERN "*.h"
  )
