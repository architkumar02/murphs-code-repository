#!/bin/bash
svn status --no-ignore | grep '^\?' | sed 's/^\?      //'  | xargs -Ixx rm -rf xx
