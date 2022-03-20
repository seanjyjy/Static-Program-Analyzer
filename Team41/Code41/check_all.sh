#!/bin/bash

cd Team41/Code41/tests || exit

autotestorPath="../cmake-build-debug/src/autotester/autotester"

num=1

open -a Firefox

for i in *; do
  if [[ $i == *_source.txt ]]; then
    queries="${i/source/queries}"
    outFile="out$num.xml"
    eval "$autotestorPath $i $queries $outFile"

    let "num+=1"
  fi
done

for i in *; do
  regex="out[0-9]+.xml"
  if [[ $i =~ $regex ]]; then
    url="http://localhost:63342/SPA/Team41/Code41/tests/$i"
    if [[ "$OSTYPE" == "darwin"* ]]; then
      /Applications/Firefox.app/Contents/MacOS/firefox -new-tab "$url"
    else
      # not sure if this is correct on other OS
      firefox -new-tab "$url"
    fi
  fi
done

exit 1