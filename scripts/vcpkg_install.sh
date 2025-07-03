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
#  qtbase[widgets,opengl] \
#  qtdeclarative \
  boost-filesystem \
  boost-asio \
  boost-property-tree \
  boost-lexical-cast \
  boost-uuid \
  --triplet "${TRIPLET}"