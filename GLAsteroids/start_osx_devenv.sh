# To run cmake we need command line tools for xcode:
# install via:
#     xcode-select --install

export REPOS="$HOME/source/repos"

# compile boost yourself
export BOOST_INCL_DIR="$REPOS/boost/boost_1_69_0"
export BOOST_LINK_DIR="$REPOS/boost/boost_1_69_0/stage/lib"

# brew install freeglut
export FREEGLUT_INCL_DIR="/System/Library/Frameworks/GLUT.framework/Headers"

export X_LINK_DIR="/usr/local/lib"
export X_INCL_DIR="$REPOS/Backend/X"

export PATH="$PATH:$X_LINK_DIR"

cmake -G Xcode ../../

# to install run
# 
#    cmake --install out/xcode --config Debug