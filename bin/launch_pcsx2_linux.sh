#!/bin/sh

# This script is a small wrapper to the PCSX2 exectuable. The purpose is to
# launch PCSX2 from the same repository every times.
# Rationale: There is no guarantee on the directory when PCSX2 is launched from a shortcut.
#            This behavior trigger the first time wizards everytime...

# Note: sh (dash on debian) does not support pushd, popd...
# Save current directory
PWD_old=$PWD

# Go to the script directory
chdir `dirname $0`
./pcsx2

# Go back to the old directory
chdir $PWD_old
