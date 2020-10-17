package main

import (
	"fmt"
	"image/color"
	"math"

	"github.com/tomnomnom/xtermcolor"
)

func main() {
	steps := 16
	cycles := 2
	gammas := []float64{0.8, 1.0, 1.2, 1.6, 2.0}

	for _, gamma := range gammas {
		fmt.Printf("Gamma %.1f -> ", gamma)
		printRainbowStrip(steps, cycles, gamma)
	}
}

func printRainbowStrip(steps int, cycles int, gamma float64) {
	for i := 0; i < steps*cycles; i++ {
		t := float64(i) / float64(steps)
		xcolor := xtermcolor.FromColor(rainbow(t, gamma))
		fmt.Printf("\x1b[48;5;%dm ", xcolor)
	}
	fmt.Printf("\x1b[0m\n")
}

func rainbow(t float64, gamma float64) color.RGBA {
	r := 0.5 + 0.5*math.Cos(2*math.Pi*(t+0.0/3))
	g := 0.5 + 0.5*math.Cos(2*math.Pi*(t+2.0/3))
	b := 0.5 + 0.5*math.Cos(2*math.Pi*(t+1.0/3))

	r = math.Pow(r, 1/gamma)
	g = math.Pow(g, 1/gamma)
	b = math.Pow(b, 1/gamma)

	r = math.Min(r*256, 255)
	g = math.Min(g*256, 255)
	b = math.Min(b*256, 255)

	return color.RGBA{uint8(r), uint8(g), uint8(b), 255}
}
