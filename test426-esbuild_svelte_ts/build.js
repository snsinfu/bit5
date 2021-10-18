#!/usr/bin/env node

import { build } from "esbuild";
import esbuildSvelte from "esbuild-svelte";
import sveltePreprocess from "svelte-preprocess";

build({
  entryPoints: ["index.ts"],
  bundle: true,
  outdir: "_out",
  plugins: [
    esbuildSvelte({
      // svelte-preprocess detects lang="ts" in svelte file and transpiles the
      // script to js using the typescript package.
      // https://github.com/sveltejs/svelte-preprocess/blob/v4.9.8/docs/preprocessing.md
      preprocess: sveltePreprocess(),
    }),
  ],
});
