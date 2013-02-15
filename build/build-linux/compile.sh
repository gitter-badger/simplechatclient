#!/bin/bash
cp /usr/bin/scc ./pkg-debian/usr/bin/scc
cp -r /usr/share/scc/* ./pkg-debian/usr/share/scc/

cd pkg-debian
find . -type f ! -regex '.*\.hg.*' ! -regex '.*?debian-binary.*' ! -regex '.*?DEBIAN.*' -printf '%P ' | xargs md5sum > DEBIAN/md5sums 
cd ..

fakeroot dpkg -b pkg-debian/ scc-1.7.0.0.deb
fakeroot alien -c -k -r scc-1.7.0.0.deb
