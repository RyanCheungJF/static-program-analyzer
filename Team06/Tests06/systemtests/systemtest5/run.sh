#!/bin/bash

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Calls.txt out-Calls.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt ErrorCheck.txt out-ErrorCheck.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Affects.txt out-Affects.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt AffectsT.txt out-AffectsT.xml
