void main(string[] args) {
    Gtk.init(ref args);

    var window = new Gtk.Window();
    window.title = "Hello, world!";
    window.set_default_size(600, 400);
    window.destroy.connect(() => Gtk.main_quit());

    var label = new Gtk.Label(null);
    label.set_markup("<big>Lorem ipsum <i>dolor sit amet</i></big>");;
    window.add(label);

    window.show_all();

    Gtk.main();
}
