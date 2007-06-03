#! /bin/bash

set -e

#python amgen.py

AM=${AUTOMAKE:-automake}
version=`$AM --version | head -n 1 | sed -e 's/.* //'`
ver=$(IFS=.; v=($version); echo $(( ${v[0]} * 100 + ${v[1]})))

if test $ver -lt 109; then
    echo "please install automake-1.9 or newer"
    exit 1
fi

AC=${ACLOCAL:-aclocal}
version=`$AC --version | head -n 1 | sed -e 's/.* //'`
ver=$(IFS=.; v=($version); echo $(( ${v[0]} * 100 + ${v[1]})))

if test $ver -lt 109; then
    echo "please use aclocal from automake-1.9 or newer"
    exit 1
fi

set -x
rm -f config.cache aclocal.m4
${ACLOCAL:-aclocal}
#${AUTOHEADER:-autoheader}
${LIBTOOLIZE:-libtoolize} --automake --copy --force
${AUTOMAKE:-automake} --add-missing --foreign
${AUTOCONF:-autoconf}
rm -rf autom4te*.cache

subdirs=

for i in $subdirs; do
  (cd $i && ./autogen.sh)
done
