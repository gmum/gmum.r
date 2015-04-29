#!/bin/bash

output="$(python 2e.py)" 


paste -d' ' <(echo "$output" | grep SCORE | sed s/"SCORE: "//)  <(echo "$output" | grep \#iter | perl -ne '/iter = (\d+)/; print "$1\n"' ) | tr ' ' '|' | sed -e 's/^/|/' -e 's/$/|/'