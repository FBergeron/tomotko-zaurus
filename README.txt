README
======

About my Zaurus
---------------
Model:       SL-C1000
ROM Version: 1.01 JP
Kernel:      2.4.20
Environment: Qtopia 1.5.4 (This is Sharp's closed-source version); Qt 2.3.2.

Some features of Qt are not included in this binary version on Qt.  To get the list of available features, take a look at the file /opt/Qtopia/include/qfeatures.h.  For example, QInputDialog is not defined.  That's why I had to implement PromptDialog.  The class has been removed since though.


How to set up development environment
-------------------------------------
In order to build the application, it's more convenient to do it on a Linux machine with a faster processor and more memory.  Using another machine to compile an application that will work on a different architecture is what we call cross-compiling.  It's probably possible to compile the application natively on the Zaurus itself but I have never considered the idea.  It already takes 42 seconds on a Core2 Duo E6300 with more than 2 GB of RAM memory, imagine on the Zaurus...

So here is what I did to set up my development environment:

I downloaded an ISO image of Debian Etch from Debian's official website (http://www.debian.org).  There is an archive section where the file debian-40r9-i386-netinst.iso can be found.  At the time I write this documentation, the url was: http://cdimage.debian.org/cdimage/archive/4.0_r9/i386/iso-cd/debian-40r9-i386-netinst.iso.

Using Virtual Box (https://www.virtualbox.org), I created a virtual machine where I installed Debian Etch.

Now for the cross-compiling setup, the main problem was to find the right documentation...  There is a lot of links everywhere but it's not always clear for which configuration and for which environment.

I've made 2 or 3 attempts to setup the cross-compiling environment.  The best link I found is this one :

http://www.ossh.com/zaurus/mirrors/docs.zaurus.com/linux_compiler_setup_howto.shtml

where I found all the required packages (see rpm files included in etc/QtopiaDevRPM).

If these links are dead, the files can also be found in the etc/QtopiaDevRPM directory.

So here is basically what I did :

$ su
$ aptitude install gcc-2.95
$ aptitude install g++-2.95
$ aptitude install alien
$ alien -i gcc-cross-sa1100-2.95.2-0.i386.rpm
$ alien -i binutils-cross-arm-2.11.2-0.i386.rpm
$ alien -i glibc-arm-2.2.2-0.i386.rpm
$ alien -i linux-headers-arm-sa1100-2.4.6-3.i386.rpm
$ alien -i qtopia-free-1.5.0-1.i386.rpm
$ aptitude install qt3-dev-tools qt3-linguist
$ aptitude install tmake
$ exit

At this moment, my setup is pretty much complete.  I can compile and test the example application and it runs either on my Debian machine or my Zaurus.  For more details, read the documention in /opt/Qtopia/doc/index.html.  

Important! I renamed/changed my $PATH to use the proper gcc/g++ (gcc on /opt/... or gcc-2.95) or manually changed it in the Makefile.  But a better approach is rather to use environment variable instead.  To do that, add the -e option when using make (e.g.: make -e).  Check bin/dev-x86-qpe.sh for details.  Also please note that using more recent version of gcc/g++ do NOT work!  That's why I'm using an old version of Debian for this because I was unable to build gcc-2.95 from source using a newer distro.

The same procedure works for an old version Ubuntu (one that provides gcc-2.95 and g++-2.95 packages).

One last thing:  The setup for zlib library must be done for the first time.  Read the instructions below.


Common operations during development
------------------------------------
To generate/update the translation file (.ts) :

$ lupdate -verbose toMOTko.pro


To compile the string files (.qm) :

$ lrelease -verbose toMOTko.pro


To generate the project file (not needed anymore but just in case):

$ progen -o toMOTko.pro


To build the Makefile :

$ tmake -o Makefile toMOTko.pro


To build the application on Linux :

$ source bin/dev-x86-qpe.sh
$ tmake -o Makefile toMOTko.pro
$ make clean
$ make -e
$ lrelease -verbose toMOTko.pro
$ su
$ source bin/dev-x86-qpe.sh
$ bin/deploy.sh
$ exit


To test the application on linux :

In a separate console :

$ source bin/dev-x86-qpe.sh
$ bin/zaurus.sh

This will launch an emulator where toMOTko application will run.


To launch the application :

In another console :

$ ./toMOTko

(The application should start if the emulator is running properly.)


To build and package the application for the zaurus :

$ source bin/dev-arm-qpe.sh
$ tmake -o Makefile toMOTko.pro
$ make clean
$ make
$ lrelease -verbose toMOTko.pro
$ su
$ source bin/dev-arm-qpe.sh
$ bin/deploy.sh
$ bin/mkipk.sh
$ chown fred.fred toMOTko_x.y.z_arm.ipk
$ exit
$ cd zaurus/toMOTko/trunk
$ scp toMOTko_x.y.z_arm.ipk zaurus@zaurus:Documents/Install_Files/.

And we uninstall and reinstall the new version.

IMPORTANT (for me only!) : To install toMOTko, it is mandatory to use ipkg.old on MY zaurus.  The latest version of ipkg that I have installed is buggy.  I should remove it but I'm a little bit scared of doing ipkg remove ipkg for obvious reasons.  Because of this, I cannot install toMOTko using Sharp's Add/Remove Applications program.  For other people, ipkg or the GUI Package Manager should work fine so substitute ipkg.old by ipkg in the following commands:

$ su
$ ipkg.old remove toMOTko
$ ipkg.old install toMOTko_x.y.z_arm.ipk


What to do before a release
---------------------------
- Update version number in the About dialog, control file and preinst script.
- Update online documentation if necessary (screenshots, new features, etc.)
- Remove traces if any.
- Build latest ipk file.
- Test it.
- Remove -DDEBUG in Makefile if it's there and test again.
- Update version number in the website (version.txt), Doxygen.
- Update website if necessary (screenshots, new features, download link, etc.)
- Run doxygen to update the documentation.
- Upload latest ipk file: cadaver https://frs.sourceforge.net/f/fb/fbergeron/uploads and then: put toMOTko_x.y.z_arm.ipk
- Update the change log file and its date (changes.log + www/en/history.shtml).
- Create the new release on SourceForge (hide it first, then use preformatted format and insert enter in the text).
- Upload the doxygen doc : cd $TOMOTKO_HOME/doc;rsync -ruv --exclude=.svn * fbergeron@web.sourceforge.net:tomotko_html/doc
- Upload the online doc : cd $TOMOTKO_HOME/help;rsync -ruv --exclude=.svn * fbergeron@web.sourceforge.net:tomotko_html/help
- Upload the latest screenshots.
- Write release notes and changes.
- Check in all changes.
- Upload new website : cd $TOMOTKO_HOME/www;rsync -ruv --exclude=.svn * fbergeron@web.sourceforge.net:tomotko_html
- Post news to announce the release.
- Close bugs in bug tracker if any.
- Do the same thing for Freshmeat, ELSI and http://www.handheld-linux.com (screenshots, release, changes).
- Tag the new version in SVN : svn copy https://tomotko.svn.sourceforge.net/svnroot/tomotko/trunk https://tomotko.svn.sourceforge.net/svnroot/tomotko/tags/release-0.X.0 -m "Tagging the 0.X.0 release."


Learned lessons and/or interesting things to know
=================================================
For a QListView that allows multiple selection, selectedItem() cannot be used.  We have to iterate through all the QListItem to find the first selected one even though we know that there is only one selected.

When changing a font of a widget, it's important to call updateGeometry() and ->layout()->invalidate() on the parent panel.  Curiously, the call order doesn't seem important...

The compiled version of Qt for Zaurus doesn't support QClipboard->setData() so I cannot use binary data in the clipboard.

Concerning setScaledContents() : It works only for pixmap not for movie (animated gif).  

To find the UTF-8 character number for a specific kanji, use this :

http://www.fileformat.info/info/unicode/char/search.htm
Enter the kanji and look for UTF-8 (hex)

To round up a positive float number in C++: uint roundedValue = (uint)( val + 0.5 );

About zlib
----------
I downloaded the source code from the zlib website (http://zlib.net).
The latest version was zlib-1.2.5.

To build the .so file, as written in the INSTALL file :
# cd ~/zlib-1.2.5
# source ../tomotko-zaurus/bin/dev-x86-qpe.sh
# ./configure --shared
# make clean;make

To install it at the right place :

# su
# cp libz.so.1.2.5 /opt/Qtopia/lib
# cd /opt/Qtopia/lib
# ln -s libz.so.1.2.5 libz.so
# ln -s libz.so.1.2.5 libz.so.1

I also had to install the header files (zlib.h, zconf.h, etc.).

# su
# cp zlib.h /opt/Qtopia/include
# cp zconf.h /opt/Qtopia/include

To use zlib library in toMOTko, I added in toMOTko.pro a reference to the library : -lz
I also added a dependency in toMOTko.control in the Depends: element.
When I build the application in x86 environment, neither the compiler or the linker complain.  It works.

After that, I need to do the same for ARM environment.  Instead of building a so file, I found one on Internet here:

http://www.lyndonhill.com/Downloads/Zaurus/libz_1.2.5_arm.ipk

I renamed it to .tgz and extracted the so files that I installed into /opt/Qtopia/sharp/lib :

# su
# cp libz.so.1.2.5 /opt/Qtopia/sharp/lib
# cd /opt/Qtopia/sharp/lib
# ln -s libz.so.1.2.5 libz.so
# ln -s libz.so.1.2.5 libz.so.1

To compile, it may be necessary to put some header files (zlib.h, zconf.h, etc.) into /opt/Qtopia/sharp/include.

For very good information about all this setup, look at the file README.zaurus of the crimson project.

zlib provides minizip (in the contrib directory) that is an implementation of zip and unzip.  Unfortunately, according to :

http://forum.winimage.com/viewtopic.php?t=100&highlight=accents+national+chars
http://forum.winimage.com/viewtopic.php?t=2056&highlight=unicode

It doesn't support utf-8 encoded filenames inside the archive.  Because of this, I decided to use numeric id for folders and vocabularies inside the archive.  It's not ideal but it's good enough.

Just for the record, on toMOTko-0.8.0, my glossary files were taking 2.6 MB and it took 16 seconds for the application to start.
After integrating zlib, the glossary files take 1.0 MB and it takes 12 seconds for the application to start.  
It's no big deal but better than nothing.  To write all the glossary files in compressed format, it took 95 seconds.  
This operation is done the first time we quit toMOTko-0.9.0.  

To generate thumbnails for screenshots, we can use a command like this one :

# cd img
> for img in *.png
> do
> convert $img -resize 50% ${img%%.png}_small.png
> done


Miscellaneous
-------------
To connect with ssh on tomotko's website:

ssh -t fbergeron,tomotko@shell.sourceforge.net create

To debug the program, I have inserted at many places #ifdef DEBUG blocks.  To enable them, edit Makefile and replace -DNO_DEBUG by -DDEBUG and recompile.
