import { oak } from "./deps.ts";

const storage: any = {};

export async function home(ctx: oak.Context) {
  const text: string = storage.text || "(empty)";
  const token: string = ctx.state.csrfToken;

  ctx.response.headers.set("content-type", "text/html");
  ctx.response.headers.set("cache-control", "no-cache");

  ctx.response.body = `<!DOCTYPE html>\n` +
    `<html lang="en">` +
    `<head><meta charset="utf-8"><title>Form</title></head>` +
    `<body>` +
    `<h1>${escapeText(text)}</h1>` +
    `<form method="post" action="/update" id="form">` +
    `<input type="text" name="text" />` +
    `<input type="submit" />` +
    `</form>` +
    `<p id="status"><p>` +
    `<script type="module">` +
    `"use strict";` +
    `const statusBox = document.querySelector("#status");` +
    `const form = document.querySelector("#form");` +
    `const textInput = form.querySelector('input[name="text"]');` +
    `const submitButton = form.querySelector('input[type="submit"]');` +
    `const tokenInput = form.querySelector('input[name="csrf_token"]');` +
    `form.addEventListener("submit", async (e) => {` +
    `e.preventDefault();` +
    `submitButton.setAttribute("disabled", "disabled");` +
    `try {` +
    `const token = (await fetch("/csrf_token")).headers.get("x-csrf-token");` +
    `const headers = { "content-type": "application/json", "x-csrf-token": token };` +
    `const body = JSON.stringify({ text: textInput.value });` +
    `await fetch(form.action, { method: form.method, headers, body });` +
    `location.reload();` +
    `} finally {` +
    `submitButton.removeAttribute("disabled");` +
    `}` +
    `});` +
    `statusBox.textContent = "Ready";` +
    `</script>` +
    `</body>` +
    `</html>`;
}

export async function tokenQuery(ctx: oak.Context) {
  ctx.response.status = 200;
  ctx.response.body = "";
}

export async function update(ctx: oak.Context) {
  const data = await ctx.request.body({ type: "json" }).value;
  storage.text = data.text;

  ctx.response.status = 200;
  ctx.response.body = "OK";
}

function escapeText(s: string): string {
  return s
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;");
}

function escapeAttribute(s: string): string {
  return s
    .replace(/&/g, "&amp;")
    .replace(/"/g, "&quot;");
}
