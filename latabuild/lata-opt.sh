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
        --disable-docs
        --disable-werror
        --enable-lata
        --insts-pattern-opt
        --indirect-jmp-opt
        --split-tb-opt
        --lata-tu-opt
	)

	pushd $srcdir/build_aarch64 >/dev/null
	../configure "${_configure64_flags[@]}"
	ninja -j$(nproc)
	popd >/dev/null
}

package() {
	mkdir -p $pkgdir/$pkgname-$pkgver/usr/{bin,lib/binfmt.d,lib/sysctl.d}
	install -Dm755 -s $srcdir/build_aarch64/lata-aarch64 $pkgdir/$pkgname-$pkgver/usr/bin/lata-aarch64
	cat >${pkgdir}/${pkgname}-${pkgver}/usr/lib/binfmt.d/lata-aarch64.conf <<EOF
:lata-aarch64:M:\x7fELF\x02\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x00\xb7\x00:\xff\xff\xff\xff\xff\xff\xff\x00\xff\xff\xff\xff\xff\xff\xff\xff\xfe\xff\xff\xff:/usr/bin/lata-aarch64:
EOF
	echo "vm.mmap_min_addr = 65536" >${pkgdir}/$pkgname-$pkgver/usr/lib/sysctl.d/mmap_min_addr.conf
	(
		cd $pkgdir
		tar Jcf $srcdir/$tarballs $pkgname-$pkgver
	)
}

parpare
build
package
