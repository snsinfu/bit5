package pack

import (
	"bytes"
)

type Writer struct {
	buf *bytes.Buffer
}

func NewWriter() *Writer {
	return &Writer{
		buf: bytes.NewBuffer(nil),
	}
}

func (w *Writer) Write(buf []byte) error {
	_, err := w.buf.Write(buf)
	return err
}

func (w *Writer) WriteByte(n byte) error {
	buf := [1]byte{n}
	return w.Write(buf[:])
}

func (w *Writer) Write16(n uint16) error {
	buf := [2]byte{
		byte(n >> 8),
		byte(n),
	}
	return w.Write(buf[:])
}

func (w *Writer) Write32(n uint32) error {
	buf := [4]byte{
		byte(n >> 24),
		byte(n >> 16),
		byte(n >> 8),
		byte(n),
	}
	return w.Write(buf[:])
}

func (w *Writer) Bytes() []byte {
	return w.buf.Bytes()
}
