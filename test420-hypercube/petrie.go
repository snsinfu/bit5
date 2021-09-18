package main

import (
	"math"
)

type Point struct {
	X float64
	Y float64
}

func Petrie(dim int, scale float64) []Point {
	max := uint8(1) << dim
	vertices := make([]Point, int(max))

	centroid := Point{}

	for bits := uint8(0); bits < max; bits++ {
		vertex := makeVertex(bits, dim, scale)
		centroid.X += vertex.X
		centroid.Y += vertex.Y
		vertices[bits] = vertex
	}

	centroid.X /= float64(max)
	centroid.Y /= float64(max)

	for i := range vertices {
		vertices[i].X -= centroid.X
		vertices[i].Y -= centroid.Y
	}

	return vertices
}

func makeVertex(bits uint8, dim int, scale float64) Point {
	vertex := Point{}

	for i := 0; i < dim; i++ {
		if (bits>>i)&1 != 0 {
			fraction := float64(i) / float64(dim)
			angle := fraction * math.Pi
			vertex.X += scale * math.Cos(angle)
			vertex.Y += scale * math.Sin(angle)
		}
	}

	return vertex
}
