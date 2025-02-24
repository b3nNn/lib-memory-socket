#!/usr/bin/bash

dir=$(dirname "$0")
rootDir=$(dirname "$dir")
cache="$dir/.cache"
sodiumName=libsodium-stable
sodiumCache="$cache/$sodiumName"
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


# Download libsodium sources
cd $cache
curl -sL https://download.libsodium.org/libsodium/releases/libsodium-1.0.20-stable.tar.gz | tar zx
cd -

cd $sodiumCache
./configure
make && make check
cd -

# Copy libsodium
cp -r "$sodiumCache/src/libsodium" "$vendorDir"

# Finally, clear the cache
rm -rf $sodiumCache
