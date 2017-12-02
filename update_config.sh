#!/bin/sh

# build-mega2560 folder may not exist, but is needed for the next steps, so
# create it if it doesn't.
mkdir build-mega2560 -p
# Store new configuration flags
echo "$1" > build-mega2560/config_cmd_record_tmp
# Compare with old configuration flags. If there is a difference, touch config.h
# to force Make to rebuild it
cmp -s build-mega2560/config_cmd_record_tmp build-mega2560/config_cmd_record
RETVAL=$?
if [ $RETVAL != 0 ]
then
    touch config.h
fi
# New configuration flags are next time's old configuration flags
echo "$1" > build-mega2560/config_cmd_record