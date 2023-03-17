#!/bin/bash

(cd systemtest1 && python3 -m http.server 8100) &
cd ..
(cd systemtest2 && python3 -m http.server 8200) &
cd ..
(cd systemtest3 && python3 -m http.server 8300) &
cd ..
(cd systemtest4 && python3 -m http.server 8400) &
cd ..
(cd systemtest5 && python3 -m http.server 8500) &
cd ..
