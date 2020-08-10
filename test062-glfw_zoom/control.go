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
}

func NewDrag() *Drag {
	return &Drag{}
}

func (d *Drag) Start(x, y int) {
	d.prevX = d.x
	d.prevY = d.y
	d.startX = x
	d.startY = y
}

func (d *Drag) Drag(x, y int) {
	d.x = d.prevX + x - d.startX
	d.y = d.prevY + y - d.startY
}

func (d *Drag) GetPos() (int, int) {
	return d.x, d.y
}
