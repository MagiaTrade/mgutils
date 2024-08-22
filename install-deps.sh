DIR="external"

if ![-d "${DIR}"; then
  mkdir "${DIR}"
fi

if [ -d "$DIR/spdlog" ]; then
  echo "$DIR spdlog exists!"
else
  echo "$DIR Downloading spdlog:"
  git clone git@github.com:gabime/spdlog.git "$DIR/spdlog"
fi

if [ -d "$DIR/catch2" ]; then
  echo "$DIR catch2 exists!"
else
  echo "$DIR Downloading catch2:"
  git clone --branch v2.x git@github.com:catchorg/Catch2.git "$DIR/catch2"
fi

if [ -d "$DIR/onetbb" ]; then
  echo "$DIR onetbb exists!"
else
  echo "$DIR Downloading onetbb:"
  git clone git@github.com:oneapi-src/oneTBB.git "$DIR/onetbb"
fi



