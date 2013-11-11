#!/bin/sh

case "$1" in
[1-9]*) colnum="$1"; shift;;
*) echo "Usage: `basename $0` colnum [files]" 1>&2; exit 1;;
esac

# Use integer output, but switch to %.4f format if "." in input.
awk '
{ sum += $col; count++; }
END { print sum / count; }
' col=$colnum OFMT='%.4f' ${1+"$@"}
