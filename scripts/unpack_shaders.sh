#!/bin/bash


SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
sh $SCRIPT_DIR/download_marathon_cli.sh

TOOL="$SCRIPT_DIR/../tools/Marathon/Marathon.CLI"

$TOOL $SCRIPT_DIR/../UnleashedRecompLib/private/shader.arc

SHADER="$SCRIPT_DIR/../UnleashedRecompLib/private/shader/xenon/shader"
SHADER_BUILD="$SCRIPT_DIR/../UnleashedRecompLib/private/shader_build"

mkdir -p $SHADER_BUILD/morph
mkdir -p $SHADER_BUILD/skin
mkdir -p $SHADER_BUILD/std

cp $SHADER/morph/no_shader.fxo $SHADER_BUILD/morph

cp $SHADER/skin/no_shader.fxo $SHADER_BUILD/skin

cp $SHADER/std/AcrobataPrimitive.fxo \
    $SHADER/std/BloomFilter.fxo \
    $SHADER/std/BurnoutBlurFilter.fxo \
    $SHADER/std/ColorCopyFilter.fxo \
    $SHADER/std/ColorCorrectionFilter.fxo \
    $SHADER/std/ColorOutFilter.fxo \
    $SHADER/std/Common.fxo \
    $SHADER/std/csd.fxo \
    $SHADER/std/csd3D.fxo \
    $SHADER/std/csdBG.fxo \
    $SHADER/std/DepthOfFieldFilter.fxo \
    $SHADER/std/en_laser00.fxo \
    $SHADER/std/font.fxo \
    $SHADER/std/MaskCopyFilter.fxo \
    $SHADER/std/Mercury.fxo \
    $SHADER/std/MotionBlurFilter.fxo \
    $SHADER/std/movieYUV420.fxo \
    $SHADER/std/no_shader.fxo \
    $SHADER/std/pe.fxo \
    $SHADER/std/PhaseShiftFilter.fxo \
    $SHADER/std/primitive.fxo \
    $SHADER/std/rt_restoration.fxo \
    $SHADER/std/SampleFilter.fxo \
    $SHADER_BUILD/std # destination

