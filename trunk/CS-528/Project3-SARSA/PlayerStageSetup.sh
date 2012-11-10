#!/bin/bash

# Script to setup PlayerStage (On Hydra Machines)
echo "Setting up Paths:"
export PLAYERPATH="/user/bin"
export STAGEPATH="/usr/bin;/usr/share/X11"
echo "PLAYERPATH: $PLAYERPATH"
echo "STAGEPATH: $STAGEPATH"

echo "To run:"
echo "[#] make"
echo "[#] robot-player Project3.cfg &"
echo "[#] ./robotAgent"
# To use Python Uncomment the following:
#PY_VER=$(python -c "import sys; print '.'.join(map(str,sys.version_inf0[:2]))")
#export PYTHONPATH="$PYTHONPATH:/usr/local/lib/python$PY_VER/site-packages"

# To use Java Uncomment the following:
#export CLASSPATH="$CLASSPAHT:/usr/local/src/javaclient/classes:."
