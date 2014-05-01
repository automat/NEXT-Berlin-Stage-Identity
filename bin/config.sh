#!/bin/bash

#Test

PATH_FILE_APP="/Users/automat/Projects/next/itr_1/Stage/xcode/build/Release/Stage.app"
PATH_FILE_CONFIG_JSON="/Users/automat/Projects/next/itr_1/Stage/resources/config.json"
PATH_FOLDER_IMAGES_SPEAKER="/Users/automat/Projects/next/data/images_dome_stage"
PATH_FOLDER_IMAGES_CLOCKS="/Users/automat/Projects/next/data/clocks"
PATH_FILE_DATA_SESSION_JSON="/Users/automat/Projects/next/data/data.json"
PATH_FILE_QUOTE_JSON="/Users/automat/Projects/next/data/quotes.json"

function OPEN_SESSION {
    killall -9 "Stage"
    open $PATH_FILE_APP --args -c $PATH_FILE_CONFIG_JSON  -d $PATH_FILE_DATA_SESSION_JSON -i $PATH_FOLDER_IMAGES_SPEAKER  -h $PATH_FOLDER_IMAGES_CLOCKS -q $PATH_FILE_QUOTE_JSON -s $1
}
