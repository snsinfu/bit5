package main

//
// Draw to a framebuffer and use the result as a texture.
//

import (
	"fmt"
	"os"
	"runtime"

	"github.com/go-gl/gl/v4.3-core/gl"
	"github.com/go-gl/glfw/v3.3/glfw"
)

const (
	windowTitle    = "Draw to texture"
	bufferWidth    = 300
	bufferHeight   = 300
	viewportWidth  = 600
	viewportHeight = 600
)

func init() {
	// OpenGL context requires main thread to be locked.
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

	// Window.

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

	window, err := glfw.CreateWindow(viewportWidth, viewportHeight, windowTitle, nil, nil)
	if err != nil {
		return err
	}

	window.MakeContextCurrent()

	// Model. A rectangle filling the entire viewport.

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
	gl.BindBuffer(gl.ARRAY_BUFFER, 0)

	// Attributes.

	const (
		attribQuadVertex = 0
		bindingQuadVertex = 0
	)

	var quadVAO uint32
	gl.GenVertexArrays(1, &quadVAO)
	defer gl.DeleteVertexArrays(1, &quadVAO)

	gl.BindVertexArray(quadVAO)
	gl.VertexAttribFormat(attribQuadVertex, 3, gl.FLOAT, false, 0)
	gl.VertexAttribBinding(attribQuadVertex, bindingQuadVertex)
	gl.BindVertexBuffer(bindingQuadVertex, quadVBO, 0, 0)
	gl.EnableVertexAttribArray(attribQuadVertex)
	gl.BindVertexArray(0)

	// Create a texture backed by a framebuffer.

	const (
		texUnit = 0
	)

	var framebuffer uint32
	gl.GenFramebuffers(1, &framebuffer)
	defer gl.DeleteFramebuffers(1, &framebuffer)

	var framebufferTex uint32
	gl.GenTextures(1, &framebufferTex)
	defer gl.DeleteTextures(1, &framebufferTex)

	gl.ActiveTexture(gl.TEXTURE0 + texUnit)
	gl.BindTexture(gl.TEXTURE_2D, framebufferTex)
	gl.TexImage2D(gl.TEXTURE_2D, 0, gl.RGBA, bufferWidth, bufferHeight, 0, gl.RGBA, gl.UNSIGNED_BYTE, gl.PtrOffset(0))

	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST)
	gl.TexParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST)

	gl.BindTexture(gl.TEXTURE_2D, 0)
	gl.ActiveTexture(gl.TEXTURE0)

	// Attach the texture to our framebuffer.
	gl.BindFramebuffer(gl.FRAMEBUFFER, framebuffer)
	gl.FramebufferTexture(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, framebufferTex, 0)

	// Set fragment shader's output location=0 to be the 0-th color attachment
	// of our framebuffer.
	drawBuffers := []uint32{gl.COLOR_ATTACHMENT0}
	gl.BindFramebuffer(gl.FRAMEBUFFER, framebuffer)
	gl.DrawBuffers(int32(len(drawBuffers)), &drawBuffers[0])

	if gl.CheckFramebufferStatus(gl.FRAMEBUFFER) != gl.FRAMEBUFFER_COMPLETE {
		return fmt.Errorf("failed to complete framebuffer")
	}

	gl.BindFramebuffer(gl.FRAMEBUFFER, 0)

	// Demo shader. This shader draws some to a texture.

	demoShader := `
#version 430 core

uniform vec2 resolution;
out vec4 fragColor;

void main() {
	vec2 st = gl_FragCoord.xy / resolution;

	fragColor.r = 0.5 + 0.5 * st.s;
	fragColor.g = 1.0 - 0.5 * st.t;
	fragColor.b = 0.5 + 0.5 * st.t;
}
`
	demoProgram, err := createFragmentShaderProgram(demoShader)
	if err != nil {
		return err
	}
	defer gl.DeleteProgram(demoProgram)

	gl.BindFragDataLocation(demoProgram, 0, gl.Str("fragColor\x00"))

	// Viewport shader. This shader renders the texture to the viewport.

	viewShader := `
#version 430 core

uniform sampler2D sampler;
uniform vec2 resolution;
out vec4 fragColor;

void main() {
	fragColor = texture(sampler, gl_FragCoord.xy / resolution);
}
`
	viewProgram, err := createFragmentShaderProgram(viewShader)
	if err != nil {
		return err
	}
	defer gl.DeleteProgram(viewProgram)

	gl.BindFragDataLocation(viewProgram, 0, gl.Str("fragColor\x00"))

	// Uniforms.

	gl.UseProgram(demoProgram)
	gl.Uniform2f(
		gl.GetUniformLocation(demoProgram, gl.Str("resolution\x00")),
		float32(bufferWidth),
		float32(bufferHeight),
	)
	gl.UseProgram(0)

	gl.UseProgram(viewProgram)
	gl.Uniform1i(
		gl.GetUniformLocation(viewProgram, gl.Str("sampler\x00")),
		texUnit,
	)
	gl.Uniform2f(
		gl.GetUniformLocation(viewProgram, gl.Str("resolution\x00")),
		float32(viewportWidth),
		float32(viewportHeight),
	)
	gl.UseProgram(0)

	// Main loop.

	window.SetKeyCallback(handleKeys)
	window.Show()

	for !window.ShouldClose() {

		gl.UseProgram(demoProgram)
		gl.BindFramebuffer(gl.FRAMEBUFFER, framebuffer)
		gl.BindVertexArray(quadVAO)
		gl.DrawArrays(gl.TRIANGLE_STRIP, 0, 4)

		gl.BindFramebuffer(gl.FRAMEBUFFER, 0)
		gl.UseProgram(viewProgram)
		gl.DrawArrays(gl.TRIANGLE_STRIP, 0, 4)

		window.SwapBuffers()

		glfw.PollEvents()
	}

	return nil
}

func handleKeys(w *glfw.Window, key glfw.Key, scancode int, action glfw.Action, mods glfw.ModifierKey) {
	if key == glfw.KeyEscape && action == glfw.Press {
		w.SetShouldClose(true)
	}
}
