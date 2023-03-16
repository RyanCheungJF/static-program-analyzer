cd systemtest1 && START /B "" python -m http.server 8100
cd ..
cd systemtest2 && START /B "" python -m http.server 8200
cd ..
cd systemtest3 && START /B "" python -m http.server 8300
cd ..
cd systemtest4 && START /B "" python -m http.server 8400
cd ..
cd systemtest5 && START /B "" python -m http.server 8500
cd ..