cd systemtest1 && START /B "" python -m http.server 8100
cd ..
cd systemtest2 && START /B "" python -m http.server 8200
cd ..