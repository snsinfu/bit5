package main


import (
	"fmt"

	"github.com/go-gl/gl/v4.3-core/gl"
)


type shaderError struct {
	Message string
	Log     string
}

func (err *shaderError) Error() string {
	return err.Message
}


func createFragmentShaderProgram(source string) (uint32, error) {
	program := gl.CreateProgram()
	if program == 0 {
		return 0, fmt.Errorf("failed to create program")
	}

	clean := func() {
		gl.DeleteProgram(program)
	}
	defer clean()

	shader := gl.CreateShader(gl.FRAGMENT_SHADER)
	if shader == 0 {
		return 0, fmt.Errorf("failed to create fragment shader")
	}
	defer gl.DeleteShader(shader)

	// Shader source needs to be passed as an array of C strings allocated in
	// a C heap. See: https://github.com/go-gl/gl/pull/44
	strs, free := gl.Strs(source + "\x00")
	defer free()

	var ok int32

	// Compile shader.
	gl.ShaderSource(shader, 1, strs, nil)
	gl.CompileShader(shader)

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

	// Link program.
	gl.AttachShader(program, shader)
	gl.LinkProgram(program)

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

	gl.DetachShader(program, shader)
	clean = func() {}

	return program, nil
}
