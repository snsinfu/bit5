(import (scheme base)
        (scheme write))

(define (greet whom)
  (write (append "Hello, " whom "!\n")))

(greet "Cyclone Scheme")
