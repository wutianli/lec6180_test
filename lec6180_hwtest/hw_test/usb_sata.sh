#!/bin/sh

#-----------------------------------------------------------------------------#
#  Test usb sata
#                                               Li Guang 2018.1.2
#-----------------------------------------------------------------------------#

echo "[`basename $0`]: into usb&sata test.............."
echo "================================================="

fdisk -l /dev/sd*

if [ $? -ne "0" ];then
    echo "[`basename $0`]: usb_sata test Error"
    exit 1
fi

exit 0