#!/bin/sh

#-----------------------------------------------------------------------------#
# WDG TEST
#                                                  Zhang Wei 2012.05.28
#-----------------------------------------------------------------------------#

#------------------------------------------------------#
#  Watch Dog Test
#------------------------------------------------------#
echo "[`basename $0`]: The System Will be Reboot ..."
sleep 5
../tools/wdg
exit 0
