package main

import (
	"errors"
	"fmt"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	iv := New(0, 1000, "")

	iv.Insert(New(0, 100, "head"))
	iv.Insert(New(900, 1000, "tail"))
	iv.Insert(New(1, 2, "just-below-headtop"))

	Show(iv)

	return nil
}

type Interval struct {
	lower *Interval
	upper *Interval
	start int64
	mid   int64
	end   int64
	label string
}

func New(start, end int64, label string) *Interval {
	return &Interval{
		start: start,
		end:   end,
		label: label,
	}
}

func (iv *Interval) Insert(sub *Interval) error {
	if sub.start < iv.start {
		return errors.New("sub interval underflows")
	}
	if sub.end > iv.end {
		return errors.New("sub interval overflows")
	}

	if iv.lower == nil {
		if iv.start == sub.start {
			iv.mid = sub.end
			iv.lower = sub
			iv.upper = New(iv.mid, iv.end, "")
			return nil
		}

		if iv.end == sub.end {
			iv.mid = sub.start
			iv.lower = New(iv.start, iv.mid, "")
			iv.upper = sub
			return nil
		}

		iv.mid = sub.end
		iv.lower = New(iv.start, iv.mid, "")
		iv.upper = New(iv.mid, iv.end, "")
	}

	if sub.start < iv.mid {
		return iv.lower.Insert(sub)
	}

	if sub.start >= iv.mid {
		return iv.upper.Insert(sub)
	}

	panic("sub interval crosses the midpoint; I have not decided how to deal with this case")
}

func Show(iv *Interval) {
	doShow(iv, 1)
}

func doShow(iv *Interval, n int) {
	mark := "-"
	if iv.lower == nil {
		mark = "*"
	}

	fmt.Printf("%*s %d:%d %s\n", n, mark, iv.start, iv.end, iv.label)

	if iv.lower != nil {
		doShow(iv.lower, n + 2)
		doShow(iv.upper, n + 2)
	}
}
