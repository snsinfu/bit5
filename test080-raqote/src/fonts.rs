use font_kit::family_name::FamilyName;
use font_kit::font::Font;
use font_kit::properties::Properties;
use font_kit::source::SystemSource;

#[derive(Debug)]
pub enum Error {
    Selection(font_kit::error::SelectionError),
    Loading(font_kit::error::FontLoadingError),
}

pub fn sans_serif() -> Result<Font, Error> {
    let family = FamilyName::SansSerif;
    let source = SystemSource::new();
    let props = Properties::new();

    let handle = match source.select_best_match(&[family], &props) {
        Ok(handle) => handle,
        Err(error) => return Err(Error::Selection(error)),
    };

    let font = match handle.load() {
        Ok(font) => font,
        Err(error) => return Err(Error::Loading(error)),
    };

    return Ok(font);
}
