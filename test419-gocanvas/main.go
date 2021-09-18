package main

import (
	"github.com/tdewolff/canvas"
	"github.com/tdewolff/canvas/renderers"
)

func main() {
	c := canvas.New(100, 100)

	ctx := canvas.NewContext(c)
	ctx.SetFillColor(canvas.Transparent)
	ctx.SetStrokeColor(canvas.Darkblue)
	ctx.SetStrokeWidth(1)
	ctx.DrawPath(50, 50, canvas.Circle(40))

	c.WriteFile("_output.svg", renderers.SVG())
}
