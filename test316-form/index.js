(function() {
  "use strict";

  let statusBox;

  class LoginForm {
    constructor(form) {
      this.form = form;
      this.button = form.querySelector('input[type="submit"]');
      this.usernameInput = form.querySelector('input[name="username"]');
      this.passwordInput = form.querySelector('input[name="password"]');
      this.setup();
    }

    setup() {
      this.form.addEventListener("submit", async (e) => {
        e.preventDefault();

        this.button.setAttribute("disabled", "disabled");
        try {
          const username = this.usernameInput.value;
          const password = this.passwordInput.value;
          await this.login(username, password);
          statusBox.textContent = "Ready";
        } finally {
          this.button.removeAttribute("disabled");
        }
      });
    }

    async login(username, password) {
      console.log("username", username);
      console.log("password", password);
      statusBox.textContent = `Logging in as ${username} (password: ${password})`;
      return new Promise((resolve, reject) => window.setTimeout(resolve, 1000));
    }
  }

  window.addEventListener("load", () => {
    statusBox = document.getElementById("status");
    new LoginForm(document.getElementById("login"));
    statusBox.textContent = "Ready";
  });
})();
