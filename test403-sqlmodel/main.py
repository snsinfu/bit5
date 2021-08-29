from typing import Optional
from sqlmodel import Field, SQLModel, Session, create_engine, select


class Note(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    text: str


def main():
    engine = create_engine("sqlite://", echo=True)
    SQLModel.metadata.create_all(engine)

    with Session(engine) as session:
        note = Note(text="The quick brown fox jumps over the lazy dog")
        session.add(note)
        session.commit()

        # https://stackoverflow.com/a/5083472
        session.refresh(note)
        note_id = note.id

    with Session(engine) as session:
        result = session.exec(select(Note).where(Note.id == note_id))
        note = result.one()

    print("")
    print("The note added:", note)


main()
