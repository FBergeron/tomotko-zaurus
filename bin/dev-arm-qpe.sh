#!/bin/bash
# dev-arm-qpe.sh script
# location : /usr/bin
# 

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

#CROSSCOMPILE=/opt/Embedix/tools:/usr/local/x86/2.95.3/bin:/opt/Embedix/tools
CROSSCOMPILE=/opt/Embedix/tools:/opt/Embedix/tools/arm-linux/bin:/usr/local/x86/2.95.3/bin:/opt/Embedix/tools
QPEDIR=/opt/Qtopia/sharp
QTDIR=/opt/Qtopia/sharp
PATH=$QTDIR/bin:$QPEDIR/bin:$CROSSCOMPILE/bin:${ORG_PATH}
TMAKEPATH=/opt/Qtopia/tmake/lib/qws/linux-sharp-g++/
LD_LIBRARY_PATH=$QTDIR/lib:${ORG_LD_LIBRARY_PATH}
export QPEDIR QTDIR PATH LD_LIBRARY_PATH TMAKEPATH PS1
echo "Altered environment for Sharp Zaurus Development ARM"
