#!/bin/bash -e

# scl devtoolset and epel repositories
yum install -y centos-release-scl epel-release

# llvm-6.0.0 repository from https://llvm.cpp.fail
cat >/etc/yum.repos.d/llvm.repo <<EOF
[llvm]
name=LLVM for CentOS-generic
baseurl=https://llvm.cpp.fail/CentOS/generic/llvm/x86_64/
enabled=1
gpgcheck=1
repo_gpgcheck=1
gpgkey=https://llvm.cpp.fail/gpg.key
EOF

LLVM_VERSION=6.0.0
yum install -y clang-${LLVM_VERSION} llvm-${LLVM_VERSION} lld-${LLVM_VERSION} \
               libc++-${LLVM_VERSION} libc++-devel-${LLVM_VERSION}
# not LLVM, but needed for builds with clang.
yum install -y libstdc++-static glibc-static

ln -s /usr/bin/clang++ /usr/bin/clang++-6.0
ln -s /usr/bin/clang-format /usr/bin/clang-format-6.0

# dependencies for bazel and build_recipes
yum install -y java-1.8.0-openjdk-devel unzip which openssl rpm-build \
               cmake3 devtoolset-4-gcc-c++ git golang libtool make ninja-build patch rsync wget \
               devtoolset-4-libatomic-devel python-virtualenv bc
yum clean all

ln -s /usr/bin/cmake3 /usr/bin/cmake
ln -s /usr/bin/ninja-build /usr/bin/ninja

# latest bazel installer
BAZEL_VERSION="$(curl -s https://api.github.com/repos/bazelbuild/bazel/releases/latest |
                  python -c "import json, sys; print json.load(sys.stdin)['tag_name']")"
BAZEL_INSTALLER="bazel-${BAZEL_VERSION}-installer-linux-x86_64.sh"
curl -OL "https://github.com/bazelbuild/bazel/releases/download/${BAZEL_VERSION}/${BAZEL_INSTALLER}"
chmod ug+x "./${BAZEL_INSTALLER}"
"./${BAZEL_INSTALLER}"
rm "./${BAZEL_INSTALLER}"

# setup bash env
echo '. scl_source enable devtoolset-4' > /etc/profile.d/devtoolset-4.sh

# enable devtoolset-4 for current shell
# disable errexit temporarily, otherwise bash will quit during sourcing
set +e
. scl_source enable devtoolset-4
set -e

EXPECTED_CXX_VERSION="g++ (GCC) 5.3.1 20160406 (Red Hat 5.3.1-6)" ./build_container_common.sh
#EXPECTED_CXX_VERSION="g++ (GCC) 7.3.1 20180303 (Red Hat 7.3.1-5)" ./build_container_common.sh
