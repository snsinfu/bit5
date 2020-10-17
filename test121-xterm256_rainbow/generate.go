package main

import (
	"fmt"
	"math"
)

func main() {
	stepsCases := []int{8, 12, 16, 24, 32}
	cycles := 2
	gamma := 1.0

	for _, steps := range stepsCases {
		printRainbowStrip(steps, cycles, gamma)

		for i := 0; i < steps; i++ {
			t := float64(i) / float64(steps)
			x := toX216(rainbow(t, gamma))
			fmt.Printf("%d ", x)
		}
		fmt.Printf("\n")
	}
}

func printRainbowStrip(steps int, cycles int, gamma float64) {
	for i := 0; i < steps*cycles; i++ {
		t := float64(i) / float64(steps)
		xcolor := toX216(rainbow(t, gamma))
		fmt.Printf("\x1b[48;5;%dm ", xcolor)
	}
	fmt.Printf("\x1b[0m\n")
}

func toX216(r, g, b float64) uint8 {
	nr := uint8(math.Min(r * 6, 5))
	ng := uint8(math.Min(g * 6, 5))
	nb := uint8(math.Min(b * 6, 5))
	return 16 + (nr * 6 + ng) * 6 + nb
}

func rainbow(t float64, gamma float64) (r, g, b float64) {
	r = t * 6
	g = t * 6 + 4
	b = t * 6 + 2

	r = math.Abs(math.Mod(r, 6) - 3) - 1
	g = math.Abs(math.Mod(g, 6) - 3) - 1
	b = math.Abs(math.Mod(b, 6) - 3) - 1

	r = math.Min(math.Max(r, 0), 1)
	g = math.Min(math.Max(g, 0), 1)
	b = math.Min(math.Max(b, 0), 1)

	r = math.Pow(r, gamma)
	g = math.Pow(g, gamma)
	b = math.Pow(b, gamma)

	return
}
