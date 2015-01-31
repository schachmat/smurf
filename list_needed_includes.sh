#!/bin/sh
(for i in include/capi/*.h; do cpp -MM -I./ "$i"; done) | sed -e 's/^.*://g' -e 's/^ //g' -e 's/ /\n/g' | sort -u
