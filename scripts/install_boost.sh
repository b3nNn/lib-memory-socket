#!/usr/bin/bash

dir=$(dirname "$0")
rootDir=$(dirname "$dir")
cache="$dir/.cache"
boostName=boost_1_87_0
boostCache="$cache/$boostName"
vendorDir="$rootDir/vendor"

# Ensure the cache directory is writable
[ -d $cache ] || mkdir $cache
if [[ ! -e $cache ]]; then
  echo "cache directory [$cache] is not writable"
  exit -1
fi

# Ensure the vendor directory is writable
[ -d $vendorDir ] || mkdir $vendorDir
if [[ ! -e $vendorDir ]]; then
  echo "vendor directory [$vendorDir] is not writable"
  exit -1
fi


# Download boost sources
cd $cache
curl -sL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz | tar zx
cd -

# Copy boost
cp -r "$boostCache/boost" "$vendorDir"

# Finally, clear the cache
rm -rf $cache
