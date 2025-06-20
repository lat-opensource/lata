#!/usr/bin/bash
set -e

pkgname=lata
pkgver=$(cat VERSION)
pkgdate=$(date +%Y%m%d)
srcdir=$(realpath "$(dirname "$0")/../")
pkgdir=$srcdir/pkg
tarballs=$pkgname-$pkgver-$pkgdate.tar.xz

parpare() {
	[ -d $srcdir/build_aarch64 ] || mkdir -p $srcdir/build_aarch64
	[ -d $pkgdir ] && rm -rf $pkgdir
	mkdir -p $pkgdir
}

build() {
	unset CXXFLAGS
	unset CFLAGS
	unset LDFLAGS
	export CFLAGS="-Wno-error=unused-but-set-variable -Wno-error=unused-function -Wformat -Werror=format-y2k"

	local _configure64_flags=(
        --target-list=aarch64-linux-user
        --enable-debug
        --enable-capstone
        --disable-werror  
        --enable-lata
        # Optionally enable optimization options for testing
        # --insts-pattern-opt
        # --indirect-jmp-opt
        # --split-tb-opt
        # --lata-tu-opt
	)

	pushd $srcdir/build_aarch64 >/dev/null
	../configure "${_configure64_flags[@]}"
	ninja -j$(nproc)
	popd >/dev/null
}

parpare
build
