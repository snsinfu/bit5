package main


import (
	"fmt"

	"github.com/go-gl/gl/v4.1-core/gl"
)


type shaderError struct {
	Message string
	Log     string
}

func (err *shaderError) Error() string {
	return err.Message
}

func createProgram(vert, frag string) (uint32, error) {
	program := gl.CreateProgram()
	if program == 0 {
		return 0, fmt.Errorf("failed to create program")
	}

	clean := func() {
		gl.DeleteProgram(program)
	}
	defer (func() { clean() })()

	// Compile shader.
	vertShader, err := compileShader(gl.VERTEX_SHADER, vert)
	if err != nil {
		return 0, err
	}
	defer gl.DeleteShader(vertShader)

	fragShader, err := compileShader(gl.FRAGMENT_SHADER, frag)
	if err != nil {
		return 0, err
	}
	defer gl.DeleteShader(fragShader)

	// Link program.
	gl.AttachShader(program, vertShader)
	gl.AttachShader(program, fragShader)
	gl.LinkProgram(program)

	var ok int32
	gl.GetProgramiv(program, gl.LINK_STATUS, &ok)
	if ok == gl.FALSE {
		err := &shaderError{Message: "shader program link error"}

		var size int32
		gl.GetProgramiv(program, gl.INFO_LOG_LENGTH, &size)
		if size > 0 {
			log := make([]byte, int(size))
			gl.GetProgramInfoLog(program, size, nil, &log[0])
			err.Log = string(log)
		}

		return 0, err
	}

	gl.DetachShader(program, vertShader)
	gl.DetachShader(program, fragShader)

	clean = func() {}

	return program, nil
}

func compileShader(xtype uint32, source string) (uint32, error) {
	shader := gl.CreateShader(xtype)
	if shader == 0 {
		return 0, fmt.Errorf("failed to create shader")
	}

	clean := func() {
		gl.DeleteShader(shader)
	}
	defer (func() { clean() })()

	// Shader source needs to be passed as an array of C strings allocated in
	// a C heap. See: https://github.com/go-gl/gl/pull/44
	strs, free := gl.Strs(source + "\x00")
	defer free()

	gl.ShaderSource(shader, 1, strs, nil)
	gl.CompileShader(shader)

	var ok int32
	gl.GetShaderiv(shader, gl.COMPILE_STATUS, &ok)
	if ok == gl.FALSE {
		err := &shaderError{Message: "shader compile error"}

		var size int32
		gl.GetShaderiv(shader, gl.INFO_LOG_LENGTH, &size)
		if size > 0 {
			log := make([]byte, int(size))
			gl.GetShaderInfoLog(shader, size, nil, &log[0])
			err.Log = string(log)
		}

		return 0, err
	}

	clean = func() {}

	return shader, nil
}
