#!/bin/bash
svn status --no-ignore | grep '^\?' | sed 's/^\?      //'
