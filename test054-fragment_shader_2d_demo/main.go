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

	gl.ClearColor(0.0, 0.0, 0.0, 0.0)

	for !window.ShouldClose() {
		gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

		window.SwapBuffers()

		glfw.PollEvents()
	}

	return nil
}
