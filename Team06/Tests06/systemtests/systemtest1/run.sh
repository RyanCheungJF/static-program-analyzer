#!/bin/bash

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Follows.txt out-Follows.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt FollowsT.txt out-FollowsT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Parent.txt out-Parent.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt ParentT.txt out-ParentT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Modifies.txt out-Modifies.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Uses.txt out-Uses.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Pattern.txt out-Pattern.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Negative.txt out-Negative.xml