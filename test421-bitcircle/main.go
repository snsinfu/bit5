package main

import (
	"github.com/tdewolff/canvas"
	"github.com/tdewolff/canvas/renderers"
)

func main() {
	figSize := 30.0
	innerRadius := 10.0
	outerRadius := 14.0
	strokeWidth := 0.2
	bits := []uint8{
		0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1,
	}

	c := canvas.New(figSize, figSize)

	bitAngle := 360 / float64(len(bits))
	bitFan := fan(innerRadius, outerRadius, bitAngle)

	ctx := canvas.NewContext(c)
	ctx.SetStrokeColor(canvas.Black)
	ctx.SetStrokeWidth(strokeWidth)

	for i, bit := range bits {
		mat := canvas.Identity
		mat = mat.Rotate((float64(i) - 0.5) * bitAngle)

		if bit == 0 {
			ctx.SetFillColor(canvas.White)
		} else {
			ctx.SetFillColor(canvas.Crimson)
		}
		ctx.DrawPath(figSize/2, figSize/2, bitFan.Transform(mat))
	}

	c.WriteFile("output.png", renderers.PNG(canvas.DPI(300)))
}

func fan(r1, r2, angle float64) *canvas.Path {
	arc := new(canvas.Path)
	arc.MoveTo(r2, 0)
	arc.Arc(r2, r2, 0, 0, angle)
	arc = arc.Reverse()
	arc.LineTo(r1, 0)
	arc.Arc(r1, r1, 0, 0, angle)
	arc.Close()
	return arc
}
