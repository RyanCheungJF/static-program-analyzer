#!/bin/bash

#../../../../../build/src/autotester/autotester source.txt MultiClause.txt out-MultiClause.xml

../../../../../build/src/autotester/autotester source.txt Calls.txt out-Calls.xml
../../../../../build/src/autotester/autotester source.txt CallsT.txt out-CallsT.xml

../../../../../build/src/autotester/autotester source.txt Next.txt out-Next.xml
../../../../../build/src/autotester/autotester source.txt NextT.txt out-NextT.xml

../../../../../build/src/autotester/autotester source.txt Modifies.txt out-Modifies.xml
../../../../../build/src/autotester/autotester source.txt Uses.txt out-Uses.xml

../../../../../build/src/autotester/autotester source.txt Affects.txt out-Affects.xml
../../../../../build/src/autotester/autotester source.txt AffectsT.txt out-AffectsT.xml

#../../../../../build/src/autotester/autotester source.txt Boolean.txt out-Boolean.xml

#../../../../../build/src/autotester/autotester source.txt Tuple.txt out-Tuple.xml
