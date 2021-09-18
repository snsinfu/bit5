package main

import (
	"image/color"

	"github.com/tdewolff/canvas"
	"github.com/tdewolff/canvas/renderers"
)

func main() {
	dimension := 5

	figSize := 400.0
	edgeSize := 100.0
	bitRadius := 3.0
	bitMargin := 1.5

	vertices := Petrie(dimension, edgeSize)
	for i := range vertices {
		vertices[i].X += figSize / 2
		vertices[i].Y += figSize / 2
	}

	c := canvas.New(figSize, figSize)

	// Edges
	edges := canvas.NewContext(c)
	edges.SetStrokeWidth(1)
	palette := Rainbow(dimension, 0.8)

	for srcId, src := range vertices {
		for i := 0; i < dimension; i++ {
			destId := srcId ^ (1 << i)
			if destId >= srcId {
				continue
			}
			dest := vertices[destId]
			edges.SetStrokeColor(palette[i])
			edges.DrawPath(src.X, src.Y, canvas.Line(dest.X-src.X, dest.Y-src.Y))
		}
	}

	// Vertex markers
	bitMarks := canvas.NewContext(c)
	bitMarks.SetStrokeColor(canvas.Black)
	bitMarks.SetStrokeWidth(1)

	for id, pos := range vertices {
		for i := 0; i < dimension; i++ {
			if (id>>i)&1 != 0 {
				bitMarks.SetFillColor(canvas.Black)
			} else {
				bitMarks.SetFillColor(canvas.White)
			}

			dx := +(bitRadius*2 + bitMargin) * (float64(i%3) - 1.0)
			dy := -(bitRadius*2 + bitMargin) * (float64(i/3) - 0.5)

			bitMarks.DrawPath(pos.X+dx, pos.Y+dy, canvas.Circle(bitRadius))
		}
	}

	c.WriteFile("output.svg", renderers.SVG())
}

func Rainbow(n int, sat float64) []color.RGBA {
	rainbow := make([]color.RGBA, n)
	for i := range rainbow {
		hue := float64(i) / float64(n)
		r, g, b := HSVToRGB(hue, sat, 1.0)
		rainbow[i] = color.RGBA{
			R: uint8(255 * r),
			G: uint8(255 * g),
			B: uint8(255 * b),
			A: 255,
		}
	}
	return rainbow
}
