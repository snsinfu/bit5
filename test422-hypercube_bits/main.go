package main

import (
	"image/color"

	"github.com/tdewolff/canvas"
	"github.com/tdewolff/canvas/renderers"
)

func main() {
	mark := markStyle{
		InnerRadius: 2.0,
		OuterRadius: 5.0,
		Color0:      canvas.White,
		Color1:      canvas.Crimson,
	}
	figSize := 250.0
	strokeWidth := 0.4
	edgeLength := 60.0
	shear := 0.0
	dimension := 4

	c := canvas.New(figSize, figSize)

	vertices := Petrie(dimension, edgeLength, shear)
	for i := range vertices {
		vertices[i].X += figSize / 2
		vertices[i].Y += figSize / 2
	}

	// Background
	{
		ctx := canvas.NewContext(c)
		ctx.SetFillColor(canvas.White)
		ctx.SetStrokeWidth(0)
		ctx.DrawPath(0, 0, canvas.Rectangle(figSize, figSize))
	}

	// Edges
	{
		ctx := canvas.NewContext(c)
		ctx.SetStrokeColor(canvas.Dimgray)
		ctx.SetStrokeWidth(strokeWidth)

		for srcId, src := range vertices {
			for i := 0; i < dimension; i++ {
				destId := srcId ^ (1 << i)
				if destId >= srcId {
					continue
				}
				dest := vertices[destId]

				ctx.SetStrokeColor(color.RGBA{
					R: uint8(srcId << (8 - dimension)),
					G: uint8(destId << (8 - dimension)),
					B: 0,
					A: 255,
				})

				ctx.DrawPath(src.X, src.Y, canvas.Line(dest.X-src.X, dest.Y-src.Y))
			}
		}
	}

	// Vertices
	{
		ctx := canvas.NewContext(c)
		ctx.SetStrokeColor(canvas.Black)
		ctx.SetStrokeWidth(strokeWidth)

		for bits, pos := range vertices {
			bitsSeq := make([]bool, dimension)
			for i := 0; i < dimension; i++ {
				if (bits>>i)&1 == 1 {
					bitsSeq[i] = true
				}
			}
			drawBinaryMark(ctx, pos.X, pos.Y, bitsSeq, mark)
		}
	}

	c.WriteFile("output.png", renderers.PNG(canvas.DPI(90)))
}

type markStyle struct {
	InnerRadius float64
	OuterRadius float64
	Color0      color.RGBA
	Color1      color.RGBA
}

func drawBinaryMark(ctx *canvas.Context, x, y float64, bits []bool, style markStyle) {
	ctx.SetFillColor(canvas.Transparent)
	ctx.DrawPath(x, y, canvas.Circle(style.InnerRadius))

	angle := 180 / float64(len(bits))
	fan := fan(style.InnerRadius, style.OuterRadius, angle)
	mat := canvas.Identity.Rotate(-0.5 * angle)

	for _, bit := range bits {
		if !bit {
			ctx.SetFillColor(style.Color0)
		} else {
			ctx.SetFillColor(style.Color1)
		}
		ctx.DrawPath(x, y, fan.Transform(mat))

		mat = mat.Rotate(angle)
	}
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
