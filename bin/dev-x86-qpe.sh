#!/bin/bash
# dev-x86-qpe.sh 
# location : /usr/bin

if [ -z ${ORG_PATH} ]
then
ORG_PATH=${PATH}
export ORG_PATH
fi

if [ -z ${ORG_LD_LIBRARY_PATH} ]
then
ORG_LD_LIBRARY_PATH=${LD_LIBRARY_PATH}
export ORG_LD_LIBRARY_PATH
fi

CROSSCOMPILE=/opt/Embedix/tools:/usr/local/x86/2.95.3/bin:/opt/Embedix/tools
QPEDIR=/opt/Qtopia
QTDIR=/opt/Qtopia
PATH=/usr/local/x86/2.95.3/bin:$QTDIR/bin:$QPEDIR/bin:${ORG_PATH}:/opt/Embedix/tools/bin
TMAKEPATH=/opt/Qtopia/tmake/lib/qws/linux-x86-g++/
LD_LIBRARY_PATH=$QTDIR/lib:${ORG_LD_LIBRARY_PATH}
export QPEDIR QTDIR PATH TMAKEPATH LD_LIBRARY_PATH PS1

#
# Do make -e to use this declaration.  Useful on some platforms.
#
CC=gcc-2.95
CXX=g++-2.95
LINK=gcc-2.95
export CC CXX LINK

echo "Altered environment for Sharp Zaurus Development x86"
