package demo

fun main() {
    val display = Display()
    val shell = Shell(display)
    shell.open()

    while (!shell.isDisposed) {
        if (!display.readAndDispatch()) {
            display.sleep()
        }
    }

    display.dispose()
}
