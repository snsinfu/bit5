from typing import Optional

from fastapi import FastAPI
from sqlmodel import Field, SQLModel, Session, create_engine, select


# Model

class Note(SQLModel, table=True):
    id: Optional[int] = Field(default=None, primary_key=True)
    text: str


engine = create_engine("sqlite://")
SQLModel.metadata.create_all(engine)


# API

app = FastAPI()


@app.get("/notes/{note_id}")
async def read_note(note_id: int):
    with Session(engine) as session:
        results = session.exec(select(Note).where(Note.id == note_id))
        note = results.one()
    return note


@app.post("/notes/")
async def create_note(note: Note):
    with Session(engine) as session:
        session.add(note)
        session.commit()
        session.refresh(note)
    return note
