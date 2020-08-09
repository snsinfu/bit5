package main

import (
	"fmt"
	"os"
	"runtime"

	"github.com/go-gl/gl/v4.1-core/gl"
	"github.com/go-gl/glfw/v3.3/glfw"
)

const (
	windowTitle  = "Resizable"
	screenWidth  = 600
	screenHeight = 600
)

// Vertex shader that passes coordinates without any transformation.
const vertShader = `
#version 410 core

in vec3 vertex;

void main() {
	gl_Position = vec4(vertex, 1);
}
`

// Fragment shader that renders some demo scene.
const fragShader = `
#version 410 core

uniform vec2 resolution;
out vec4 fragColor;

void main() {
	vec2 st = gl_FragCoord.xy / resolution;

	fragColor.r = 0.5 + 0.5 * st.s;
	fragColor.g = 1.0 - 0.5 * st.t;
	fragColor.b = 0.5 + 0.5 * st.t;
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
	// Window

	if err := glfw.Init(); err != nil {
		return err
	}
	defer glfw.Terminate()

	glfw.WindowHint(glfw.Visible, glfw.False)
	glfw.WindowHint(glfw.Resizable, glfw.True)
	glfw.WindowHint(glfw.ContextVersionMajor, 4)
	glfw.WindowHint(glfw.ContextVersionMinor, 1)
	glfw.WindowHint(glfw.OpenGLProfile, glfw.OpenGLCoreProfile)
	glfw.WindowHint(glfw.OpenGLForwardCompatible, glfw.True)

	window, err := glfw.CreateWindow(screenWidth, screenHeight, windowTitle, nil, nil)
	if err != nil {
		return err
	}

	viewportWidth, viewportHeight := window.GetFramebufferSize()

	window.MakeContextCurrent()

	// OpenGL

	if err := gl.Init(); err != nil {
		return err
	}

	// A rectangle filling the entire viewport

	quadVertices := []float32{
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0,
	}

	var quadVBO uint32
	gl.GenBuffers(1, &quadVBO)
	defer gl.DeleteBuffers(1, &quadVBO)

	gl.BindBuffer(gl.ARRAY_BUFFER, quadVBO)
	gl.BufferData(gl.ARRAY_BUFFER, 4*len(quadVertices), gl.Ptr(quadVertices), gl.STATIC_DRAW)

	// Attributes

	const attribQuadVertex = 0

	var quadVAO uint32
	gl.GenVertexArrays(1, &quadVAO)
	defer gl.DeleteVertexArrays(1, &quadVAO)

	gl.BindVertexArray(quadVAO)
	gl.BindBuffer(gl.ARRAY_BUFFER, quadVBO)
	gl.VertexAttribPointer(attribQuadVertex, 3, gl.FLOAT, false, 0, gl.PtrOffset(0));
	gl.EnableVertexAttribArray(attribQuadVertex)

	// Demo shader. This shader draws something to a texture.

	program, err := createProgram(vertShader, fragShader)
	if err != nil {
		return err
	}
	defer gl.DeleteProgram(program)

	gl.BindFragDataLocation(program, 0, gl.Str("fragColor\x00"))

	// Uniforms

	gl.UseProgram(program)
	gl.Uniform2f(
		gl.GetUniformLocation(program, gl.Str("resolution\x00")),
		float32(viewportWidth),
		float32(viewportHeight),
	)

	// Main loop

	window.SetFramebufferSizeCallback(func(window *glfw.Window, w, h int) {
		viewportWidth = w
		viewportHeight = h

		gl.UseProgram(program)
		gl.Uniform2f(
			gl.GetUniformLocation(program, gl.Str("resolution\x00")),
			float32(viewportWidth),
			float32(viewportHeight),
		)
	})

	window.Show()

	for !window.ShouldClose() {
		gl.Viewport(0, 0, int32(viewportWidth), int32(viewportHeight))
		gl.UseProgram(program)
		gl.DrawArrays(gl.TRIANGLE_STRIP, 0, int32(len(quadVertices)))

		window.SwapBuffers()

		glfw.PollEvents()
	}

	return nil
}
