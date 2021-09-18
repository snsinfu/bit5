package main

import (
	"math"
)

func HSVToRGB(h, s, v float64) (r, g, b float64) {
	p := rem(h, 1) * 3
	q := rem(p, 1) * 2

	x := 1.0
	y := 1.0
	z := 1.0 - s
	if q > 1 {
		x -= s * (q - 1)
	} else {
		y -= s * (1 - q)
	}

	r, g, b = rotate3(x, y, z, -int(p))
	r *= v
	g *= v
	b *= v

	return
}

func rem(x, y float64) float64 {
	r := math.Mod(x, y)
	if r < 0 {
		return r + y
	}
	return r
}

func rotate3(x, y, z float64, n int) (u, v, w float64) {
	if n > 0 {
		return rotate3(y, z, x, n-1)
	}
	if n < 0 {
		return rotate3(z, x, y, n+1)
	}
	return x, y, z
}
