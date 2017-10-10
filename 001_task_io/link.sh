# edit the link to your xmc4500_template folder
LINK="../../workspace_examples/xmc4500_template"

#ln -s $LINK/inc inc
ln -s $LINK/system system
ln -s $LINK/xmclib xmclib

#ln $LINK/doxyfile
#ln $LINK/Makefile

cp $LINK/doxyfile doxyfile
cp $LINK/Makefile Makefile

chmod +x Makefile doxyfile

mkdir inc
mkdir bin
mkdir src
mkdir doc
