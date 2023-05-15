#!/bin/sh
###############################################################################
##
## Copyright (C) 2014-2016, New York University.
## Copyright (C) 2011-2014, NYU-Poly.
## Copyright (C) 2006-2011, University of Utah.
## All rights reserved.
## Contact: contact@vistrails.org
##
## This file is part of VisTrails.
##
## "Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##
##  - Redistributions of source code must retain the above copyright notice,
##    this list of conditions and the following disclaimer.
##  - Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
##  - Neither the name of the New York University nor the names of its
##    contributors may be used to endorse or promote products derived from
##    this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
## THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
## PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
## CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
## EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
## OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
## WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
## OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
## ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
##
###############################################################################

# The script fixes up the links to make the matplotlib installation
# self-contained in the VisTrails.app bundle

# Note that you'll need to get the libpng12.0.dylib and
# libfreetype.6.dylib libaries (perhaps from an XQuartz installation)
# and then copy them to <bin_dir>/Contents/Frameworks before running
# the script

BIN_PATH_25="Contents/Resources/lib/python2.5"
BIN_PATH_26="Contents/Resources/lib/python2.6"
BIN_PATH_27="Contents/Resources/lib/python2.7"
FRAMEWORK_PATH="@executable_path/../Frameworks"
X11_PATH="/usr/X11/lib"

MPL_LIB_DIR="matplotlib"
PNG_LIB="libpng12.0.dylib"
PNG_REFS="_png.so"
FT_LIB="libfreetype.6.dylib"
FT_REFS="backends/_backend_agg.so backends/_tkagg.so ft2font.so"

if [ -z "$1" ]
then
    echo "usage: $0 <bin_dir>"
    exit 65
fi

if [ -e "$1/$BIN_PATH_27" ]
then
    BIN_PATH=$BIN_PATH_27
elif [ -e "$1/$BIN_PATH_26" ]
then
    BIN_PATH=$BIN_PATH_26
elif [ -e "$1/$BIN_PATH_25" ]
then
    BIN_PATH=$BIN_PATH_25
fi

for lib in $PNG_REFS
do
    install_name_tool -change $X11_PATH/$PNG_LIB $FRAMEWORK_PATH/$PNG_LIB $1/$BIN_PATH/$MPL_LIB_DIR/$lib
done

for lib in $FT_REFS
do
    install_name_tool -change $X11_PATH/$FT_LIB $FRAMEWORK_PATH/$FT_LIB $1/$BIN_PATH/$MPL_LIB_DIR/$lib
done

