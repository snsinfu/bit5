package main

import (
	"fmt"
	"os"
	"runtime"

	"github.com/go-gl/gl/v4.1-core/gl"
	"github.com/go-gl/glfw/v3.3/glfw"
	_ "github.com/go-gl/mathgl/mgl32"
)

const (
	windowWidth = 800
	windowHeight = 600
	windowTitle = "Shader"
)

func init() {
	// Need this to prevent exception saying "nextEventMatchingMask should only
	// be called from the Main Thread!"
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
	defer glfw.Terminate();

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

	if err := gl.Init(); err != nil {
		return err
	}

	// Surface quad. We directly feed a 2D quad vertices defined in the viewport
	// coordinates so we can omit vertex shader.
	// TODO.

	vertexCoords := []float32{
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0,
	}

	vertexTriangles := []uint32{
		0, 1, 3,
		0, 2, 3,
	}

	_ = vertexCoords
	_ = vertexTriangles

	// Surface framebuffer and texture.
	//
	// TODO: I want to allow fragment shader to read previous pixels to compute
	// new pixel values. Maybe I need to use two buffers and swap these on each
	// frame.

	var surfaceFBO uint32
	gl.GenFramebuffers(1, &surfaceFBO)
	gl.BindFramebuffer(gl.FRAMEBUFFER, surfaceFBO)

	var surfaceTex uint32
	gl.GenTextures(1, &surfaceTex)
	gl.BindTexture(gl.TEXTURE_2D, surfaceTex)
	gl.TexImage2D(gl.TEXTURE_2D, 0, gl.RGB, windowWidth, windowHeight, 0, gl.RGB, gl.UNSIGNED_BYTE, nil)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)

	gl.FramebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, surfaceTex, 0)

	if gl.CheckFramebufferStatus(gl.FRAMEBUFFER) != gl.FRAMEBUFFER_COMPLETE {
		return fmt.Errorf("framebuffer not complete")
	}

	// Fragment shader.
	// TODO: Load from file.

	fragShader := `
#version 330 core

uniform sampler2D surface;

void main() {
	gl_FragColor = texture(surface, gl_FragCoords.xy);
}
`

	_ = fragShader

	// Main loop.

	gl.ClearColor(0.0, 0.0, 0.0, 0.0)

	for !window.ShouldClose() {
		gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

		window.SwapBuffers()

		glfw.PollEvents()
	}

	return nil
}
