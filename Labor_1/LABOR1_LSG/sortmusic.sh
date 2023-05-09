#!/bin/sh
#######################################################################
# Sample solution for Lab 1 exercise "sortmusic"
# Usage:
#   sortmusic.sh --year
# Copies all MP3s in this directory to subdirectories with the year.
#
# Author: Rainer Keller, HS Esslingen
# Date: 2022-03-15
#######################################################################

function show_help {
    echo "Usage: sortmusic.sh [-h|--help] [-a|--artist] [-d|--disc] [-y|--year]"
    echo ""
    echo "Copies the MP3s into a sub-directories according to the sorting criteria."
    echo ""
    echo "  -h|--help: Show this help and quits"
    echo "  -a|--artist: Copy MP3s in this directory by artist name's first letter"
    echo "  -d|--disc: Copy MP3s in this directory by album disc name's first letter"
    echo "  -y|--year: Copy MP3s in this directory by album's four digit release year"
    echo ""
}

OPTIONS=hady
LONGOPTS=help,artist,disc,year

OPTIND=1         # Reset in case getopts has been used previously in the shell.

ATTRIBUTE=""

### MacOS getopt does not allow --long-options
#  while getopt "$OPTIONS" --long-options=$LONGOPTS; do
while getopts "$OPTIONS" opt; do
    case "$opt" in
    h|help)
        show_help
        return
        ;;
    a|artist)
        ATTRIBUTE="Artist: "
        CHAR_RANGE="50-72"
        KEY_LEN=1
        ;;
    d|disc)
        ATTRIBUTE="^Album  :"
        CHAR_RANGE="10-41"
        KEY_LEN=1
        ;;
    y|year)
        ATTRIBUTE="Year: [0-9]{4}"
        CHAR_RANGE="48-51"
        KEY_LEN=4
    esac
done

shift $((OPTIND-1))

if test x"$ATTRIBUTE" = "x" ; then
    show_help
    return
fi


for i in *.mp3 ; do
    VALUE=`id3v2 --list "$i" | grep -E "$ATTRIBUTE" | cut -c $CHAR_RANGE`
    KEY=`echo $VALUE | cut -c1-$KEY_LEN`
    # echo "VALUE:$VALUE"
    # Parameter -p for parent is important, otherwise mkdir complains
    mkdir -p $KEY
    cp "$i" $KEY
done

