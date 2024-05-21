# THIS FILE WAS AUTO-GENERATED
#
#  $ lcitool manifest ci/manifest.yml
#
# https://gitlab.com/libvirt/libvirt-ci

FROM docker.io/library/alpine:edge

RUN apk update && \
    apk upgrade && \
    apk add \
        ca-certificates \
        ccache \
        check \
        gcc \
        gettext \
        git \
        glib-dev \
        gobject-introspection-dev \
        gtk-doc \
        hwids-pci \
        hwids-usb \
        libsoup-dev \
        libxml2-dev \
        libxml2-utils \
        libxslt-dev \
        make \
        meson \
        musl-dev \
        osinfo-db-tools \
        pkgconf \
        py3-lxml \
        py3-pytest \
        py3-requests \
        python3 \
        samurai \
        vala \
        wget \
        xz && \
    rm -f /usr/lib*/python3*/EXTERNALLY-MANAGED && \
    apk list --installed | sort > /packages.txt && \
    mkdir -p /usr/libexec/ccache-wrappers && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/cc && \
    ln -s /usr/bin/ccache /usr/libexec/ccache-wrappers/gcc

ENV CCACHE_WRAPPERSDIR "/usr/libexec/ccache-wrappers"
ENV LANG "en_US.UTF-8"
ENV MAKE "/usr/bin/make"
ENV NINJA "/usr/bin/ninja"
ENV PYTHON "/usr/bin/python3"
