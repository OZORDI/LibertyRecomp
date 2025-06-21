#!/bin/bash


SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
sh $SCRIPT_DIR/download_marathon_cli.sh

TOOL="$SCRIPT_DIR/../tools/Marathon/Marathon.CLI"


SHADER="$SCRIPT_DIR/../UnleashedRecompLib/private/shader/xenon/shader"
PRIVATE="$SCRIPT_DIR/../UnleashedRecompLib/private"
mkdir -p $PRIVATE/shader

cp $PRIVATE/shader.arc $PRIVATE/shader_lt.arc $PRIVATE/shader

$TOOL $PRIVATE/shader/shader.arc
$TOOL $PRIVATE/shader/shader_lt.arc

rm $PRIVATE/shader/shader.arc $PRIVATE/shader/shader_lt.arc
