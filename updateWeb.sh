#!/bin/bash

gzip -9kf ./web/css/csss.css
xxd -i ./web/css/csss.css.gz ./src/webserver/csss.h

gzip -9kf ./web/js/jss.js
xxd -i ./web/js/jss.js.gz ./src/webserver/jss.h

gzip -9kf ./web/js/jss2.js
xxd -i ./web/js/jss2.js.gz ./src/webserver/jss2.h

gzip -9kf ./web/js/jss3.js
xxd -i ./web/js/jss3.js.gz ./src/webserver/jss3.h

gzip -9kf ./web/index.html
xxd -i ./web/index.html.gz ./src/webserver/index.h

gzip -9kf ./web/Blockly.html
xxd -i ./web/Blockly.html.gz ./src/webserver/blockly.h
rm ./web/*.gz
rm ./web/js/*.gz
rm ./web/css/*.gz
sed -i '1i\const '  ./src/webserver/csss.h
sed -i '1i\const ' ./src/webserver/jss.h
sed -i '1i\const ' ./src/webserver/jss2.h
sed -i '1i\const ' ./src/webserver/jss3.h
sed -i '1i\const ' ./src/webserver/index.h
sed -i '1i\const ' ./src/webserver/blockly.h
