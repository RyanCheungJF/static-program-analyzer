#!/bin/bash

../../../Code06/cmake-build-release/src/autotester/autotester source.txt Calls.txt out-Calls.xml
../../../Code06/cmake-build-release/src/autotester/autotester source.txt CallsT.txt out-CallsT.xml

../../../Code06/cmake-build-release/src/autotester/autotester source.txt ErrorCheck.txt out-ErrorCheck.xml
../../../Code06/cmake-build-release/src/autotester/autotester source.txt Pattern.txt out-Pattern.xml

../../../Code06/cmake-build-release/src/autotester/autotester source.txt Affects.txt out-Affects.xml
../../../Code06/cmake-build-release/src/autotester/autotester source.txt AffectsT.txt out-AffectsT.xml

../../../Code06/cmake-build-release/src/autotester/autotester source.txt Boolean.txt out-Boolean.xml

../../../Code06/cmake-build-release/src/autotester/autotester source.txt Tuple.txt out-Tuple.xml

../../../Code06/cmake-build-release/src/autotester/autotester source.txt With.txt out-With.xml
