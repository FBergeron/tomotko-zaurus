#!/bin/sh
source bin/dev-x86-qpe.sh
/opt/Qtopia/bin/qvfb -width 640 -height 480 &
#/opt/Qtopia/bin/qvfb -width 480 -height 640 &
sleep 2
/opt/Qtopia/bin/qpe &
exit 0
