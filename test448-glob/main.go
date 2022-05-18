package main

import (
	"fmt"
)

func main() {
	pat := "*.example.com"

	fmt.Println(Match(pat, "www.example.com"))
	fmt.Println(Match(pat, "api.example.org"))
	fmt.Println(Match(pat, "example.org"))
	fmt.Println(Match(pat, "example.com"))
	fmt.Println(Match(pat, ".example.com"))
	fmt.Println(Match(pat, "data-*.example.com"))
	fmt.Println(Match(pat, "*"))
	fmt.Println(Match(pat, ""))
}

func Match(s, t string) bool {
	const wildChar = '*'

	rows := len(s) + 1
	cols := len(t) + 1
	match := make([]bool, rows*cols)

	match[0] = true

	for i := 1; i < rows; i++ {
		row := i * cols
		prevRow := row - cols
		if s[i-1] == wildChar {
			match[row] = match[prevRow]
		}
	}

	for j := 1; j < cols; j++ {
		col := j
		prevCol := col - 1
		if t[j-1] == wildChar {
			match[col] = match[prevCol]
		}
	}

	for i := 1; i < rows; i++ {
		for j := 1; j < cols; j++ {

			b := s[i-1]
			c := t[j-1]

			row := i * cols
			col := j
			prevRow := row - cols
			prevCol := col - 1

			if b == wildChar || c == wildChar {
				match[row+col] = match[prevRow+col] || match[row+prevCol]
			} else {
				match[row+col] = match[prevRow+prevCol] && b == c
			}
		}
	}

	return match[len(s)*cols+len(t)]
}
