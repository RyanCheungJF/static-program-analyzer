#!/bin/bash

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Follows.txt out-Follows.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt FollowsT.txt out-FollowsT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Parent.txt out-Parent.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt ParentT.txt out-ParentT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Pattern.txt out-Pattern.xml