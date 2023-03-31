#!/bin/bash

../../../../../build/src/autotester/autotester source.txt Calls.txt out-Calls.xml
../../../../../build/src/autotester/autotester source.txt CallsT.txt out-CallsT.xml
../../../../../build/src/autotester/autotester source.txt ErrorCheck.txt out-ErrorCheck.xml
../../../../../build/src/autotester/autotester source.txt Pattern.txt out-Pattern.xml
../../../../../build/src/autotester/autotester source.txt Affects.txt out-Affects.xml
../../../../../build/src/autotester/autotester source.txt AffectsT.txt out-AffectsT.xml
../../../../../build/src/autotester/autotester source.txt With.txt out-With.xml
