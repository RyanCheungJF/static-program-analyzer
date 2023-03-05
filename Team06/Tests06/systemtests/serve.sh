#!/bin/bash

(cd systemtest1 && python3 -m http.server 8100) &
(cd systemtest2 && python3 -m http.server 8200) &
(cd systemtest3 && python3 -m http.server 8300) &
