package main

import (
	"fmt"
	"os"
	"runtime"
	"time"
	"unsafe"

	"github.com/go-gl/gl/v4.1-core/gl"
	"github.com/go-gl/glfw/v3.3/glfw"
)

// I want my graphics code to be compatible with WebGL. The current stable WebGL
// version is 2.0, which is based on OpenGL ES 3.0, which in turn is compatible
// with OpenGL 4.3.
//
// https://en.wikipedia.org/wiki/WebGL
// https://en.wikipedia.org/wiki/OpenGL_ES#OpenGL_ES_3.0

const (
	windowWidth  = 500
	windowHeight = 500
	windowTitle  = "Shader"
)

func init() {
	// glfw event loop must run in the main thread.
	runtime.LockOSThread()
}

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	if err := gl.Init(); err != nil {
		return err
	}

	// Window. Needs to be created before building an OpenGL scene.

	if err := glfw.Init(); err != nil {
		return err
	}
	defer glfw.Terminate()

	glfw.WindowHint(glfw.Visible, glfw.False)
	glfw.WindowHint(glfw.Resizable, glfw.False)
	glfw.WindowHint(glfw.ContextVersionMajor, 4)
	glfw.WindowHint(glfw.ContextVersionMinor, 3)
	glfw.WindowHint(glfw.OpenGLProfile, glfw.OpenGLCoreProfile)
	glfw.WindowHint(glfw.OpenGLForwardCompatible, glfw.True)

	window, err := glfw.CreateWindow(windowWidth, windowHeight, windowTitle, nil, nil)
	if err != nil {
		return err
	}

	window.MakeContextCurrent()

	// Model. Here, I construct a quad (rectangle) spanning the entire viewport
	// as a pair of triangles sharing an edge. Vertices are packed as triangle
	// strips. See: https://en.wikipedia.org/wiki/Triangle_strip

	surfaceVertices := []float32{
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0,
	}

	var surfaceVBO uint32
	gl.GenBuffers(1, &surfaceVBO)
	defer gl.DeleteBuffers(1, &surfaceVBO)

	gl.BindBuffer(gl.ARRAY_BUFFER, surfaceVBO)
	gl.BufferData(gl.ARRAY_BUFFER, 4*len(surfaceVertices), unsafe.Pointer(&surfaceVertices[0]), gl.STATIC_DRAW)

	// Shader. Since the quad vertices are passed in screen coordinates, I only
	// attach a fragment shader and omit a vertex shader.

	fragShaderSource := `
#version 430 core

uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;
out vec4 fragColor;

void main() {
	vec2 st = gl_FragCoord.xy / resolution;

	float u = cos(st.x + time);
	float v = sin(st.y + time);
	u *= u;
	v *= v;

	fragColor.r = 0.5 + 0.5 * u;
	fragColor.g = 1.0 - 0.5 * v;
	fragColor.b = 0.5 + 0.5 * v;

	float r = length(gl_FragCoord.xy - mouse.xy);
	float mask = 1 - clamp(35 - r, 0, 1);
	fragColor *= mask;
}
`

	program := gl.CreateProgram()
	defer gl.DeleteProgram(program)

	gl.BindFragDataLocation(program, 0, gl.Str("fragColor\x00"))

	{
		fragShader := gl.CreateShader(gl.FRAGMENT_SHADER)
		if fragShader == 0 {
			return fmt.Errorf("failed to create fragment shader")
		}
		defer gl.DeleteShader(fragShader)

		// Shader source needs to be passed as an array of C strings allocated
		// in a C heap. See: https://github.com/go-gl/gl/pull/44
		strs, free := gl.Strs(fragShaderSource + "\x00")
		defer free()

		gl.ShaderSource(fragShader, 1, strs, nil)
		gl.CompileShader(fragShader)

		var ok int32

		gl.GetShaderiv(fragShader, gl.COMPILE_STATUS, &ok)
		if ok == gl.FALSE {
			var logSize int32
			gl.GetShaderiv(fragShader, gl.INFO_LOG_LENGTH, &logSize)

			if logSize <= 0 {
				return fmt.Errorf("fragment shader compile error")
			}

			log := make([]byte, int(logSize))
			gl.GetShaderInfoLog(fragShader, logSize, nil, &log[0])
			return fmt.Errorf("fragment shader compile error\n%s", string(log))
		}

		gl.AttachShader(program, fragShader)
		gl.LinkProgram(program)

		gl.GetProgramiv(program, gl.LINK_STATUS, &ok)
		if ok == gl.FALSE {
			var logSize int32
			gl.GetProgramiv(program, gl.INFO_LOG_LENGTH, &logSize)

			if logSize <= 0 {
				return fmt.Errorf("program link error")
			}

			log := make([]byte, int(logSize))
			gl.GetProgramInfoLog(program, logSize, nil, &log[0])
			return fmt.Errorf("program link error\n%s", string(log))
		}

		gl.DetachShader(program, fragShader)
	}

	// Attributes. VAO is unique per an OpenGL context, hence no Bind*/Use* API
	// unlike buffers or programs.

	const (
		attribSurfaceVertex = 0
	)

	var vao uint32
	gl.GenVertexArrays(1, &vao)
	defer gl.DeleteVertexArrays(1, &vao)

	gl.BindVertexArray(vao)
	gl.BindBuffer(gl.ARRAY_BUFFER, surfaceVBO)
	gl.VertexAttribPointer(attribSurfaceVertex, 3, gl.FLOAT, false, 0, unsafe.Pointer(nil))
	gl.EnableVertexAttribArray(attribSurfaceVertex)

	// Uniforms.

	gl.UseProgram(program)

	uniformResolution := gl.GetUniformLocation(program, gl.Str("resolution\x00"))
	uniformMouse := gl.GetUniformLocation(program, gl.Str("mouse\x00"))
	uniformTime := gl.GetUniformLocation(program, gl.Str("time\x00"))

	gl.Uniform2f(uniformResolution, float32(windowWidth), float32(windowHeight))
	gl.Uniform2f(uniformMouse, 0.0, 0.0)
	gl.Uniform1f(uniformTime, 0.0)

	// Main loop.

	const (
		maxFPS = 60
	)

	window.SetKeyCallback(handleKeys)
	window.Show()

	startTime := time.Now()
	prevTime := startTime

	gl.Uniform1f(gl.GetUniformLocation(program, gl.Str("time\x00")), 0.0)

	for !window.ShouldClose() {

		// glfw.PollEvents returns immediately if no event is queued. Manually
		// control update interval to maintain consistent FPS.
		curTime := time.Now()
		delaySec := float64(curTime.Sub(prevTime)) / float64(time.Second)

		if delaySec*maxFPS >= 1 {
			prevTime = curTime

			// Render a frame. No need to call gl.Clear() since a quad surface
			// is rendered to the entire viewport.

			gl.UseProgram(program)

			// Mouse position is measured from the upper left corner. Transform
			// it to fragment coordinates (lower left origin and 0.5-based).
			mouseX, mouseY := window.GetCursorPos()
			mouseY = windowHeight - mouseY
			gl.Uniform2f(uniformMouse, float32(mouseX)+0.5, float32(mouseY)+0.5)

			timeSec := float64(curTime.Sub(startTime)) / float64(time.Second)
			gl.Uniform1f(uniformTime, float32(timeSec))

			// Draw a quad.
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
