package main

type Zoom struct {
	level int
}

func NewZoom() *Zoom {
	return &Zoom{}
}

type Drag struct {
	x int
	y int
	prevX int
	prevY int
	startX int
	startY int

	minX int
	maxX int
	minY int
	maxY int
}

func NewDrag() *Drag {
	return &Drag{}
}

func (d *Drag) SetXBounds(min, max int) {
	d.minX = min
	d.maxX = max
}

func (d *Drag) SetYBounds(min, max int) {
	d.minY = min
	d.maxY = max
}

func (d *Drag) Start(x, y int) {
	x = clamp(x, d.minX, d.maxX)
	y = clamp(y, d.minY, d.maxY)

	d.prevX = d.x
	d.prevY = d.y
	d.startX = x
	d.startY = y
}

func (d *Drag) Drag(x, y int) {
	d.x = d.prevX + x - d.startX
	d.y = d.prevY + y - d.startY

	d.x = clamp(d.x, d.minX, d.maxX)
	d.y = clamp(d.y, d.minY, d.maxY)
}

func (d *Drag) GetPos() (int, int) {
	return d.x, d.y
}

func clamp(x, min, max int) int {
	if x < min {
		return min
	}
	if x > max {
		return max
	}
	return x
}
