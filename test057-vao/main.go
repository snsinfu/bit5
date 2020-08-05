package main

import (
	"fmt"
	"os"
	"runtime"

	"github.com/go-gl/gl/v4.1-core/gl"
	"github.com/go-gl/glfw/v3.3/glfw"
)

const (
	viewportWidth  = 500
	viewportHeight = 500
	windowTitle    = "Shader"

	// 4.1 is the maximum version supported by macOS.
	openglMajor = 4
	openglMinor = 1

	maxFPS = 60
)

const vertShader = `
#version 410

in vec3 vertex;

void main() {
	gl_Position = vec4(vertex, 1);
}
`

const fragShader = `
#version 410

uniform vec2 resolution;
out vec4 fragColor;

void main() {
	vec2 st = gl_FragCoord.xy / resolution;

	fragColor.r = 0.5 + 0.5 * st.x;
	fragColor.g = 1.0 - 0.5 * st.y;
	fragColor.b = 0.5 + 0.5 * st.y;
}
`

func init() {
	runtime.LockOSThread()
}

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {

	// Window.

	if err := glfw.Init(); err != nil {
		return err
	}
	defer glfw.Terminate();

	glfw.WindowHint(glfw.Visible, glfw.False)
	glfw.WindowHint(glfw.Resizable, glfw.False)
	glfw.WindowHint(glfw.ContextVersionMajor, openglMajor)
	glfw.WindowHint(glfw.ContextVersionMinor, openglMinor)
	glfw.WindowHint(glfw.OpenGLProfile, glfw.OpenGLCoreProfile)
	glfw.WindowHint(glfw.OpenGLForwardCompatible, glfw.True)

	window, err := glfw.CreateWindow(viewportWidth, viewportHeight, windowTitle, nil, nil)
	if err != nil {
		return err
	}

	window.MakeContextCurrent()

	// OpenGL

	if err := gl.Init(); err != nil {
		return err
	}

	version := gl.GoStr(gl.GetString(gl.VERSION))
	fmt.Println("OpenGL", version)

	// Quad VBO.

	quadVertex := []float32{
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0,
	}

	var vbo uint32
	gl.GenBuffers(1, &vbo)
	defer gl.DeleteBuffers(1, &vbo)

	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.BufferData(gl.ARRAY_BUFFER, 4*len(quadVertex), gl.Ptr(quadVertex), gl.STATIC_DRAW)
	gl.BindBuffer(gl.ARRAY_BUFFER, 0)

	// Shader program.

	program, err := newProgram(vertShader, fragShader)
	if err != nil {
		return err
	}
	defer gl.DeleteProgram(program)

	gl.BindFragDataLocation(program, 0, gl.Str("fragColor\x00"))

	gl.UseProgram(program)
	gl.Uniform2f(
		gl.GetUniformLocation(program, gl.Str("resolution\x00")),
		float32(viewportWidth),
		float32(viewportHeight),
	)
	gl.UseProgram(0)

	// VAO. Use VertexAttribPointer in OpenGL < 4.3.

	var vao uint32
	gl.GenVertexArrays(1, &vao)
	defer gl.DeleteVertexArrays(1, &vao)

	gl.BindVertexArray(vao)

	attribVertex := uint32(gl.GetAttribLocation(program, gl.Str("vertex\x00")))
	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.VertexAttribPointer(attribVertex, 3, gl.FLOAT, false, 0, gl.PtrOffset(0))
	gl.EnableVertexAttribArray(attribVertex)

	gl.BindVertexArray(0)

	// Main loop.

	window.SetKeyCallback(handleKeys)
	window.Show()

	gl.ClearColor(0, 0, 0, 1)
	gl.Clear(gl.COLOR_BUFFER_BIT)

	prevTime := glfw.GetTime()

	for !window.ShouldClose() {

		curTime := glfw.GetTime() 
		delay := curTime - prevTime

		if delay*maxFPS >= 1 {
			prevTime = curTime

			gl.UseProgram(program)
			gl.BindVertexArray(vao)
			gl.DrawArrays(gl.TRIANGLE_STRIP, 0, 4)

			window.SwapBuffers()
		}

		glfw.PollEvents()
	}

	return nil
}

func handleKeys(w *glfw.Window, key glfw.Key, scancode int, action glfw.Action, mods glfw.ModifierKey) {
	if key == glfw.KeyEscape && action == glfw.Press {
		w.SetShouldClose(true)
	}
}
