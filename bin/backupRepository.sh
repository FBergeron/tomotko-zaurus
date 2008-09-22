#!/bin/sh
echo Enter latest revision number.
read rev
mkdir tomotko-r$rev-svn-backup
rsync -av tomotko.svn.sourceforge.net::svn/tomotko/* tomotko-r$rev-svn-backup
tar zcf tomotko-r$rev-svn-backup.tar.gz tomotko-r$rev-svn-backup
rm -rf tomotko-r$rev-svn-backup
