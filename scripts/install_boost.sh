#!/usr/bin/bash

dir=$(dirname "$0")
rootDir=$(dirname "$dir")
cache="$dir/.cache"
boostName=boost_1_87_0
boostCache="$cache/$boostName"
vendorDir="$rootDir/vendor"
vendorBoostDir="$rootDir/vendor/boost"

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


# Ensure the boost vendor directory is writable
[ -d $vendorBoostDir ] || mkdir $vendorBoostDir
if [[ ! -e $vendorBoostDir ]]; then
  echo "vendor boost directory [$vendorBoostDir] is not writable"
  exit -1
fi

libs="$vendorBoostDir/libs"
boost="$vendorBoostDir/boost"

[ -d $libs ] || mkdir -p $libs
if [[ ! -e $libs ]]; then
  echo "vendor boost libs directory [$libs] is not writable"
  exit -1
fi

[ -d $boost ] || mkdir -p $boost
if [[ ! -e $boost ]]; then
  echo "vendor boost main directory [$boost] is not writable"
  exit -1
fi

# Download boost sources
cd $cache
curl -sL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz | tar zx
@cd -

# Copy interprocess files
cp -r "$boostCache/boost/interprocess" "$vendorBoostDir/boost/interprocess"
cp -r "$boostCache/libs/interprocess" "$vendorBoostDir/libs/interprocess"

# Copy lockfree files
cp -r "$boostCache/boost/lockfree" "$vendorBoostDir/boost/lockfree"
cp -r "$boostCache/libs/lockfree" "$vendorBoostDir/libs/lockfree"

