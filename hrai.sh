#!/bin/bash

INSTALLED=
PATH_TO_BIN=$(dirname `which $0`)
[ -z "$1" ] && exit 1

if [ -n "$INSTALLED" ] ; then
    CMD_BINARY=$PATH_TO_BIN/../libexec/$1
else
    CMD_BINARY=$PATH_TO_BIN/$1
fi
shift

[ ! -f $CMD_BINARY ] && exit 1

$CMD_BINARY "$@"
exit $?
