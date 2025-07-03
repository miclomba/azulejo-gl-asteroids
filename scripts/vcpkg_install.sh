export VCPKG_BUILD_TYPE=release

# Determine host OS and pick the vcpkg triplet
case "$(uname -s)" in
  Darwin)
    TRIPLET="x64-osx"
    ;;
  Linux)
    TRIPLET="x64-linux"
    ;;
  *)
    echo "Unsupported OS: $(uname -s)" >&2
    exit 1
    ;;
esac

vcpkg install --classic \
  boost-filesystem \
  boost-asio \
  boost-property-tree \
  boost-lexical-cast \
  boost-uuid \
  freeglut \
  qtbase[widgets,opengl] \
  qtdeclarative \
  --triplet "${TRIPLET}"