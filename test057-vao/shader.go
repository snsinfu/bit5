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

func newProgram(vert, frag string) (uint32, error) {
	program := gl.CreateProgram()
	if program == 0 {
		return 0, fmt.Errorf("failed to create program")
	}

	clean := func() {
		gl.DeleteProgram(program)
	}
	defer (func() { clean() })()

	vertShader, err := newShader(gl.VERTEX_SHADER, vert)
	if err != nil {
		return 0, err
	}
	defer gl.DeleteShader(vertShader)

	fragShader, err := newShader(gl.FRAGMENT_SHADER, frag)
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

		var logSize int32
		gl.GetProgramiv(program, gl.INFO_LOG_LENGTH, &logSize)
		if logSize > 0 {
			log := make([]byte, int(logSize))
			gl.GetProgramInfoLog(program, logSize, nil, &log[0])
			err.Log = string(log)
		}

		return 0, err
	}

	gl.DetachShader(program, vertShader)
	gl.DetachShader(program, fragShader)
	clean = func() {}

	return program, nil
}

func newShader(xtype uint32, source string) (uint32, error) {
	shader := gl.CreateShader(xtype)
	if shader == 0 {
		return 0, fmt.Errorf("failed to create fragment shader")
	}

	clean := func() {
		gl.DeleteShader(shader)
	}
	defer (func() { clean() })()

	// Shader source needs to be passed as an array of C strings allocated in
	// a C heap. See: https://github.com/go-gl/gl/pull/44
	strs, free := gl.Strs(source + "\x00")
	defer free()

	// Compile shader.
	gl.ShaderSource(shader, 1, strs, nil)
	gl.CompileShader(shader)

	var ok int32
	gl.GetShaderiv(shader, gl.COMPILE_STATUS, &ok)
	if ok == gl.FALSE {
		err := &shaderError{Message: "fragment shader compile error"}

		var logSize int32
		gl.GetShaderiv(shader, gl.INFO_LOG_LENGTH, &logSize)
		if logSize > 0 {
			log := make([]byte, int(logSize))
			gl.GetShaderInfoLog(shader, logSize, nil, &log[0])
			err.Log = string(log)
		}

		return 0, err
	}

	clean = func() {}

	return shader, nil
}
