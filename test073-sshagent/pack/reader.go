package pack

import (
	"bytes"
	"errors"
)

var ErrShortInput = errors.New("short input")


type Reader struct {
	buf *bytes.Reader
}

func NewReader(buf []byte) *Reader {
	return &Reader{
		buf: bytes.NewReader(buf),
	}
}

func (r *Reader) Read(buf []byte) (int, error) {
	return r.buf.Read(buf)
}

func (r *Reader) ReadExact(buf []byte) error {
	n, err := r.buf.Read(buf)
	if err != nil {
		return err
	}
	if n != len(buf) {
		return ErrShortInput
	}
	return nil
}

func (r *Reader) ReadByte() (byte, error) {
	var buf [1]byte
	err := r.ReadExact(buf[:])
	return buf[0], err
}

func (r *Reader) Read16() (uint16, error) {
	var buf [2]byte
	err := r.ReadExact(buf[:])
	if err != nil {
		return 0, err
	}
	val := (uint16(buf[0]) << 8) | uint16(buf[1])
	return val, nil
}

func (r *Reader) Read32() (uint32, error) {
	var buf [4]byte
	err := r.ReadExact(buf[:])
	if err != nil {
		return 0, err
	}
	val := (uint32(buf[0]) << 24) | (uint32(buf[1]) << 16) | (uint32(buf[2]) << 8) | uint32(buf[3])
	return val, nil
}
