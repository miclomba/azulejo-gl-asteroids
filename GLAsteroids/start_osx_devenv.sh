# To run cmake we need command line tools for xcode:
# install via:
#     xcode-select --install

export REPOS="$HOME/source/repos"
export GLASTEROIDS_DIR="$REPOS/GLAsteroids/GLAsteroids"
export X_DIR="$REPOS/Backend/X"

export INSTALL_PATH="$GLASTEROIDS_DIR/out/xcode/install"
export GLASTEROIDS_LINK_DIR="$GLASTEROIDS_DIR/out/xcode"
export GLASTEROIDS_INCL_DIR="$GLASTEROIDS_DIR"

# compile boost yourself
export BOOST_INCL_DIR="$REPOS/boost/boost_1_69_0"
export BOOST_LINK_DIR="$REPOS/boost/boost_1_69_0/stage/lib"

# brew install freeglut
export FREEGLUT_INCL_DIR="/System/Library/Frameworks/GLUT.framework/Headers"
export FREEGLUT_LINK_LIB=""
export FREEGLUT_LINK_BIN=""

export X_LINK_DIR="$X_DIR/out/xcode/install"
export X_INCL_DIR="$X_DIR"

export PATH="$PATH:$X_LINK_DIR"

cmake -G Xcode ../../

# to install run
# 
#    cmake --install out/xcode --config Debug