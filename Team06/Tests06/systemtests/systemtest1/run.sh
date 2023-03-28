#!/bin/bash

cd $(dirname $0)
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Select.txt out-Select.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Follows.txt out-Follows.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt FollowsT.txt out-FollowsT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Parent.txt out-Parent.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt ParentT.txt out-ParentT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Modifies.txt out-Modifies.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Uses.txt out-Uses.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Calls.txt out-Calls.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Pattern.txt out-Pattern.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Negative.txt out-Negative.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt MultiClause.txt out-MultiClause.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Calls.txt out-Calls.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt CallsT.txt out-CallsT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Next.txt out-Next.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt NextT.txt out-NextT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Affects.txt out-Affects.xml
../../../Code06/cmake-build-debug/src/autotester/autotester source.txt AffectsT.txt out-AffectsT.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Boolean.txt out-Boolean.xml

../../../Code06/cmake-build-debug/src/autotester/autotester source.txt Tuple.txt out-Tuple.xml
