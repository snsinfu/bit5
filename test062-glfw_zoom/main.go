package main

import (
	"fmt"
	"os"
	"runtime"

	"github.com/go-gl/gl/v3.3-core/gl"
	"github.com/go-gl/glfw/v3.3/glfw"
)

const (
	windowTitle  = "Scroll"
	windowWidth  = 600
	windowHeight = 600
)

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
	if err := glfw.Init(); err != nil {
		return err
	}
	defer glfw.Terminate()

	glfw.WindowHint(glfw.Visible, glfw.True)
	glfw.WindowHint(glfw.Resizable, glfw.False)
	glfw.WindowHint(glfw.ContextVersionMajor, 4)
	glfw.WindowHint(glfw.ContextVersionMinor, 1)
	glfw.WindowHint(glfw.OpenGLProfile, glfw.OpenGLCoreProfile)
	glfw.WindowHint(glfw.OpenGLForwardCompatible, glfw.True)

	window, err := glfw.CreateWindow(windowWidth, windowHeight, windowTitle, nil, nil)
	if err != nil {
		return err
	}

	window.MakeContextCurrent()

	viewportWidth, viewportHeight := window.GetFramebufferSize()

	// Set up demo scene
	if err := gl.Init(); err != nil {
		return err
	}

	quadCoords := []float32{
		-1, -1, 0, 1, -1, 0, -1, 1, 0, 1, 1, 0,
	}

	var vbo uint32
	gl.GenBuffers(1, &vbo)
	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.BufferData(gl.ARRAY_BUFFER, 4*len(quadCoords), gl.Ptr(quadCoords), gl.STATIC_DRAW)

	const vertSrc = `
#version 330
in vec3 vertex;
void main() {
	gl_Position = vec4(vertex, 1);
}
`
	vertSrcs, vertFree := gl.Strs(vertSrc + "\x00")
	defer vertFree()

	const fragSrc = `
#version 330
uniform vec2 resolution;
out vec4 fragColor;
void main() {
	vec2 st = gl_FragCoord.xy / resolution;
	fragColor.r = 0.5 + 0.5 * st.x;
	fragColor.g = 1.0 - 0.5 * st.y;
	fragColor.b = 0.5 + 0.5 * st.y;
}
`
	fragSrcs, fragFree := gl.Strs(fragSrc + "\x00")
	defer fragFree()

	vertShader := gl.CreateShader(gl.VERTEX_SHADER)
	gl.ShaderSource(vertShader, 1, vertSrcs, nil)
	gl.CompileShader(vertShader)

	fragShader := gl.CreateShader(gl.FRAGMENT_SHADER)
	gl.ShaderSource(fragShader, 1, fragSrcs, nil)
	gl.CompileShader(fragShader)

	program := gl.CreateProgram()
	gl.AttachShader(program, vertShader)
	gl.AttachShader(program, fragShader)
	gl.BindAttribLocation(program, 0, gl.Str("vertex\x00"))
	gl.BindFragDataLocation(program, 0, gl.Str("fragColor\x00"))
	gl.LinkProgram(program)

	// VAO
	var vao uint32
	gl.GenVertexArrays(1, &vao)
	defer gl.DeleteVertexArrays(1, &vao)

	gl.BindVertexArray(vao)
	gl.BindBuffer(gl.ARRAY_BUFFER, vbo)
	gl.VertexAttribPointer(0, 3, gl.FLOAT, false, 0, gl.PtrOffset(0))
	gl.EnableVertexAttribArray(0)

	// Uniform
	gl.UseProgram(program)
	gl.Uniform2f(
		gl.GetUniformLocation(program, gl.Str("resolution\x00")),
		float32(viewportWidth),
		float32(viewportHeight),
	)

	// Main loop
	drag := NewDrag()
	grabbed := false

	window.SetScrollCallback(func(w *glfw.Window, x, y float64) {
		fmt.Printf("SCROLL (%g, %g)\n", x, y)
	})

	window.SetCursorPosCallback(func(w *glfw.Window, x, y float64) {
		if grabbed {
			drag.Drag(int(x), viewportHeight-int(y))
		}
	})

	window.SetMouseButtonCallback(func(w *glfw.Window, button glfw.MouseButton, action glfw.Action, mods glfw.ModifierKey) {
		if button == glfw.MouseButtonLeft {
			switch action {
			case glfw.Press:
				grabbed = true
				x, y := window.GetCursorPos()
				drag.Start(int(x), viewportHeight-int(y))

			case glfw.Release:
				grabbed = false
			}
		}
	})

	gl.ClearColor(0, 0, 0, 1)

	for !window.ShouldClose() {
		gl.Clear(gl.COLOR_BUFFER_BIT)

		x, y := drag.GetPos()
		gl.Viewport(
			int32(x), int32(y), int32(viewportWidth), int32(viewportHeight),
		)
		gl.UseProgram(program)
		gl.BindVertexArray(vao)
		gl.DrawArrays(gl.TRIANGLE_STRIP, 0, 4)

		window.SwapBuffers()
		glfw.PollEvents()
	}

	return nil
}
