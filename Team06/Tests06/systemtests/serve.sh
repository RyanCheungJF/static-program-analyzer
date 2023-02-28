#!/bin/bash

(cd systemtest1 && python3 -m http.server 8100) &
(cd systemtest1 && python3 -m http.server 8200) &