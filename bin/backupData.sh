#!/bin/sh

cd ~/bak
scp -r zaurus@zaurus:.toMOTko .
tar zcf tomotko.tar.gz .toMOTko
rm -rf .toMOTko
