# THIS FILE WAS AUTO-GENERATED
#
#  $ lcitool manifest ci/manifest.yml
#
# https://gitlab.com/libvirt/libvirt-ci

function install_buildenv() {
    export DEBIAN_FRONTEND=noninteractive
    apt-get update
    apt-get dist-upgrade -y
    apt-get install --no-install-recommends -y \
            ca-certificates \
            ccache \
            check \
            gcc \
            gettext \
            git \
            gtk-doc-tools \
            hwdata \
            libc6-dev \
            libgirepository1.0-dev \
            libglib2.0-dev \
            libsoup2.4-dev \
            libxml2-dev \
            libxml2-utils \
            libxslt1-dev \
            locales \
            make \
            meson \
            ninja-build \
            osinfo-db-tools \
            pkgconf \
            python3 \
            python3-lxml \
            python3-pytest \
            python3-requests \
            valac \
            wget \
            xz-utils
    sed -Ei 's,^# (en_US\.UTF-8 .*)$,\1,' /etc/locale.gen
    dpkg-reconfigure locales
    rm -f /usr/lib*/python3*/EXTERNALLY-MANAGED
    dpkg-query --showformat '${Package}_${Version}_${Architecture}\n' --show > /packages.txt
    mkdir -p /usr/libexec/ccache-wrappers
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/cc
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/gcc
}

export CCACHE_WRAPPERSDIR="/usr/libexec/ccache-wrappers"
export LANG="en_US.UTF-8"
export MAKE="/usr/bin/make"
export NINJA="/usr/bin/ninja"
export PYTHON="/usr/bin/python3"
