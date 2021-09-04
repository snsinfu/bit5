interface Note {
  text: string;
  creation: number;
}

interface AppData {
  notes: Note[];
}

function init() {
  const data = loadAppData();

  const updateList = () => {
    const list = document.getElementById("notes")!;
    while (list.lastChild) {
      list.removeChild(list.lastChild);
    }
    for (const note of data.notes) {
      const item = document.createElement("li");
      item.textContent = note.text;
      list.appendChild(item);
    }
  };
  updateList();

  document.getElementById("save")!.addEventListener("click", () => {
    data.notes.push({
      text: (document.getElementById("note")! as HTMLInputElement).value,
      creation: Date.now(),
    });
    saveAppData(data)
    updateList();
  });
}


function loadAppData(): AppData {
  const data = window.localStorage.getItem("appData");
  if (data) {
    return JSON.parse(data);
  }
  return {notes: []};
}

function saveAppData(data: AppData) {
  window.localStorage.setItem("appData", JSON.stringify(data));
}

window.addEventListener("load", init);
