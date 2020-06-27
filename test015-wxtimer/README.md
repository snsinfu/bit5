# wxWidgets event-driven MVC

- Model: Counter
- Controller: App (wxApp)
- View: View (wxFrame)

```
            read                 update           graphics
 Counter  ------->     App      -------->  View  ---------->  user
  model   <-------  controller                   <----------
           update       ^                   |      input
                        | event             |
                        |                   | event
  timer  -------> wx event queue <----------+
          event
```

View translates user input to a custom domain-specific event and sends it to
App. App changes Counter state accordingly. Asynchronously, App periodically
reads Counter state using its own timer and updates View.
