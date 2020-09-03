use raqote::{DrawOptions, DrawTarget, PathBuilder, Point, SolidSource, Source, StrokeStyle};

mod fonts;

fn main() {
    let mut target = DrawTarget::new(300, 150);

    let edge = {
        let mut builder = PathBuilder::new();
        builder.move_to(40., 80.);
        builder.cubic_to(110., 100., 190., 100., 260., 80.);
        builder.finish()
    };

    let node1 = {
        let mut builder = PathBuilder::new();
        builder.arc(40., 80., 20., 0., 2. * std::f32::consts::PI);
        builder.finish()
    };

    let node2 = {
        let mut builder = PathBuilder::new();
        builder.arc(260., 80., 20., 0., 2. * std::f32::consts::PI);
        builder.finish()
    };

    let arrowhead = {
        let mut builder = PathBuilder::new();
        builder.move_to(230., 80.);
        builder.line_to(240., 85.);
        builder.line_to(233., 93.);
        builder.finish()
    };

    let sans_serif = match fonts::sans_serif() {
        Ok(font) => font,
        Err(error) => {
            eprintln!("error: {:?}", error);
            std::process::exit(1);
        }
    };

    // Edge connecting the nodes
    target.stroke(
        &edge,
        &Source::Solid(SolidSource {
            r: 0x10,
            g: 0x10,
            b: 0x10,
            a: 0xff,
        }),
        &StrokeStyle::default(),
        &DrawOptions::default(),
    );

    // Node 1
    target.fill(
        &node1,
        &Source::Solid(SolidSource {
            r: 0xff,
            g: 0xff,
            b: 0xff,
            a: 0xff,
        }),
        &DrawOptions::default(),
    );

    target.stroke(
        &node1,
        &Source::Solid(SolidSource {
            r: 0x10,
            g: 0x10,
            b: 0x10,
            a: 0xff,
        }),
        &StrokeStyle::default(),
        &DrawOptions::default(),
    );

    target.draw_text(
        &sans_serif,
        16.,
        "A",
        Point::new(34.5, 86.),
        &Source::Solid(SolidSource {
            r: 0x10,
            g: 0x10,
            b: 0x10,
            a: 0xff,
        }),
        &DrawOptions::default(),
    );

    // Node 2
    target.fill(
        &node2,
        &Source::Solid(SolidSource {
            r: 0xff,
            g: 0xff,
            b: 0xff,
            a: 0xff,
        }),
        &DrawOptions::default(),
    );

    target.stroke(
        &node2,
        &Source::Solid(SolidSource {
            r: 0x10,
            g: 0x10,
            b: 0x10,
            a: 0xff,
        }),
        &StrokeStyle::default(),
        &DrawOptions::default(),
    );

    target.draw_text(
        &sans_serif,
        16.,
        "B",
        Point::new(255., 86.),
        &Source::Solid(SolidSource {
            r: 0x10,
            g: 0x10,
            b: 0x10,
            a: 0xff,
        }),
        &DrawOptions::default(),
    );

    // Arrowhead
    target.stroke(
        &arrowhead,
        &Source::Solid(SolidSource {
            r: 0x10,
            g: 0x10,
            b: 0x10,
            a: 0xff,
        }),
        &StrokeStyle::default(),
        &DrawOptions::default(),
    );

    // Render to PNG
    if let Err(error) = target.write_png("output.png") {
        eprintln!("error: {:?}", error);
        std::process::exit(1);
    }
}
